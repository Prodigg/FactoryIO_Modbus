#include "vision.h"
#include <stdexcept>

FactoryIO::vision_t::vision_t(
	ModbusProvider_t& mb, 
	visionMode_t mode, 
	modbusAddr_t singleDetectionAddr, 
	modbusAddr_t bit0Addr, 
	modbusAddr_t bit1Addr, 
	modbusAddr_t bit2Addr, 
	modbusAddr_t bit3Addr, 
	modbusAddr_t bitfieldAddr, 
	modbusAddr_t IDAddr) : 
	_mb(mb), 
	_mode(mode), 
	_singleDetectionAddr(singleDetectionAddr),
	_bit0Addr(bit0Addr),
	_bit1Addr(bit1Addr),
	_bit2Addr(bit2Addr),
	_bit3Addr(bit3Addr),
	_bitfieldAddr(bitfieldAddr),
	_IDAddr(IDAddr) { }

FactoryIO::vision_t FactoryIO::vision_t::constructDetectBase(ModbusProvider_t& mb, modbusAddr_t singleDetectionAddr) {
	return vision_t(
		mb, 
		visionMode_t::DETECT_BASE, 
		singleDetectionAddr, 
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR
	);
}

FactoryIO::vision_t FactoryIO::vision_t::constructDetectLid(ModbusProvider_t& mb, modbusAddr_t singleDetectionAddr) {
	return vision_t(
		mb,
		visionMode_t::DETECT_LID,
		singleDetectionAddr,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR
	);
}

FactoryIO::vision_t FactoryIO::vision_t::constructDetectRaw(ModbusProvider_t& mb, modbusAddr_t singleDetectionAddr) {
	return vision_t(
		mb,
		visionMode_t::DETECT_RAW,
		singleDetectionAddr,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR
	);
}

FactoryIO::vision_t FactoryIO::vision_t::constructDetectAllDigital(ModbusProvider_t& mb, modbusAddr_t bit0Addr, modbusAddr_t bit1Addr, modbusAddr_t bit2Addr, modbusAddr_t bit3Addr) {
	return vision_t(
		mb,
		visionMode_t::ALL_DIGITAL,
		NO_MODBUS_ADDR,
		bit0Addr,
		bit1Addr,
		bit2Addr,
		bit3Addr,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR
	);
}

FactoryIO::vision_t FactoryIO::vision_t::constructDetectNumerical(ModbusProvider_t& mb, modbusAddr_t bitfieldAddr) {
	return vision_t(
		mb,
		visionMode_t::ALL_NUMERICAL,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		bitfieldAddr,
		NO_MODBUS_ADDR
	);
}

FactoryIO::vision_t FactoryIO::vision_t::constructDetectID(ModbusProvider_t& mb, modbusAddr_t IDAddr) {
	return vision_t(
		mb,
		visionMode_t::ALL_ID,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		IDAddr
	);
}

bool FactoryIO::vision_t::isPartPresent() {
	if (_mode == visionMode_t::DETECT_BASE || _mode == visionMode_t::DETECT_LID || _mode == visionMode_t::DETECT_RAW) {
		internal::checkModbusAddr(_singleDetectionAddr);
		return _mb.readInputBit(_singleDetectionAddr);
	}

	if (_mode == visionMode_t::ALL_ID) {
		return getPartID() != 0;
	}

	if (_mode == visionMode_t::ALL_NUMERICAL) {
		internal::checkModbusAddr(_bitfieldAddr);
		return _mb.readInputRegister(_bitfieldAddr) != 0;
	}

	if (_mode == visionMode_t::ALL_DIGITAL) {
		internal::checkModbusAddr(_bit0Addr);
		internal::checkModbusAddr(_bit1Addr);
		internal::checkModbusAddr(_bit2Addr);
		internal::checkModbusAddr(_bit3Addr);
		if (_mb.readInputBit(_bit0Addr)) return true;
		if (_mb.readInputBit(_bit1Addr)) return true;
		if (_mb.readInputBit(_bit2Addr)) return true;
		if (_mb.readInputBit(_bit3Addr)) return true;
		return false;
	}

	return false;
}

bool FactoryIO::vision_t::isLidPresent() {
	Parts_t detectedPart = getDetectedPart();
	return detectedPart == Parts_t::BLUE_PRODUCT_LID || detectedPart == Parts_t::GREEN_PRODUCT_LID || detectedPart == Parts_t::METTAL_PRODUCT_LID;
}

bool FactoryIO::vision_t::isBasePresent() {
	Parts_t detectedPart = getDetectedPart();
	return detectedPart == Parts_t::BLUE_PRODUCT_BASE || detectedPart == Parts_t::GREEN_PRODUCT_BASE || detectedPart == Parts_t::METTAL_PRODUCT_BASE;
}

bool FactoryIO::vision_t::isRawPresent() {
	Parts_t detectedPart = getDetectedPart();
	return detectedPart == Parts_t::BLUE_PRODUCT_BASE || detectedPart == Parts_t::GREEN_PRODUCT_BASE || detectedPart == Parts_t::METTAL_PRODUCT_BASE;
}

uint16_t FactoryIO::vision_t::getPartID() {
	if (_mode != visionMode_t::ALL_ID)
		throw std::domain_error("feature not avalable due to wrong mode");
	internal::checkModbusAddr(_IDAddr);
	return _mb.readInputRegister(_IDAddr);
}

FactoryIO::Parts_t FactoryIO::vision_t::getDetectedPart() {
	if (!(_mode == visionMode_t::ALL_NUMERICAL || _mode == visionMode_t::ALL_DIGITAL))
		throw std::domain_error("freature not avalable due to wrong mode");
	if (_mode == visionMode_t::ALL_NUMERICAL)
		return getPartNumerical();
	if (_mode == visionMode_t::ALL_DIGITAL)
		return getPartDigital();
	throw std::runtime_error("unreachable state reached");
}

FactoryIO::Parts_t FactoryIO::vision_t::getPartDigital() {
	internal::checkModbusAddr(_bit0Addr);
	internal::checkModbusAddr(_bit1Addr);
	internal::checkModbusAddr(_bit2Addr);
	internal::checkModbusAddr(_bit3Addr);

	byte numericalValue = 0;
	numericalValue |= _mb.readInputBit(_bit0Addr) << 0;
	numericalValue |= _mb.readInputBit(_bit1Addr) << 1;
	numericalValue |= _mb.readInputBit(_bit2Addr) << 2;
	numericalValue |= _mb.readInputBit(_bit3Addr) << 3;

	return getPartNumerical(numericalValue);
}

FactoryIO::Parts_t FactoryIO::vision_t::getPartNumerical() {
	internal::checkModbusAddr(_bitfieldAddr);
	return getPartNumerical(_mb.readInputRegister(_bitfieldAddr));
}

FactoryIO::Parts_t FactoryIO::vision_t::getPartNumerical(uint16_t partNumber) {
	switch (partNumber) {
	case 0: 
		return Parts_t::NO_PART;
	case 1: 
		return Parts_t::BLUE_RAW_MATERIAL;
	case 2:
		return Parts_t::BLUE_PRODUCT_LID;
	case 3:
		return Parts_t::BLUE_PRODUCT_BASE;
	case 4:
		return Parts_t::GREEN_RAW_MATERIAL;
	case 5:
		return Parts_t::GREEN_PRODUCT_LID;
	case 6:
		return Parts_t::GREEN_PRODUCT_BASE;
	case 7:
		return Parts_t::METTAL_RAW_MATERIAL;
	case 8:
		return Parts_t::METTAL_PRODUCT_LID;
	case 9:
		return Parts_t::METTAL_PRODUCT_BASE;
	default:
		throw std::runtime_error("unreachable state reached.");
	}
}

