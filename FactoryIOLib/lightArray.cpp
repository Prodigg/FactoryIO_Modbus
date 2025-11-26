#include "lightArray.h"
#include <stdexcept>

FactoryIO::lightArray_t::lightArray_t(
	ModbusProvider_t& mb,
	lightArrayMode_t mode,
	modbusAddr_t valueAddr,
	modbusAddr_t beam1Addr,
	modbusAddr_t beam2Addr,
	modbusAddr_t beam3Addr,
	modbusAddr_t beam4Addr,
	modbusAddr_t beam5Addr,
	modbusAddr_t beam6Addr,
	modbusAddr_t beam7Addr,
	modbusAddr_t beam8Addr,
	modbusAddr_t beam9Addr,
	modbusAddr_t analogInput,
	uint16_t scaleFactor) :
	_mb(mb),
	_mode(mode),
	_valueAddr(valueAddr), 
	_beam1Addr(beam1Addr),
	_beam2Addr(beam2Addr),
	_beam3Addr(beam3Addr),
	_beam4Addr(beam4Addr),
	_beam5Addr(beam5Addr),
	_beam6Addr(beam6Addr),
	_beam7Addr(beam7Addr),
	_beam8Addr(beam8Addr),
	_beam9Addr(beam9Addr),
	_analogInput(analogInput), 
	_scaleFactor(scaleFactor) { }

FactoryIO::lightArray_t FactoryIO::lightArray_t::constructNumerical(ModbusProvider_t& mb, modbusAddr_t valueAddr) {
	return lightArray_t(
		mb,
		lightArrayMode_t::NUMERICAL, 
		valueAddr, 
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		0
	);
}

FactoryIO::lightArray_t FactoryIO::lightArray_t::constructDigital(
	ModbusProvider_t& mb, 
	modbusAddr_t beam1Addr, 
	modbusAddr_t beam2Addr, 
	modbusAddr_t beam3Addr, 
	modbusAddr_t beam4Addr, 
	modbusAddr_t beam5Addr, 
	modbusAddr_t beam6Addr, 
	modbusAddr_t beam7Addr, 
	modbusAddr_t beam8Addr, 
	modbusAddr_t beam9Addr) {
	return lightArray_t(
		mb, 
		lightArrayMode_t::DIGITAL,
		NO_MODBUS_ADDR, 
		beam1Addr,
		beam2Addr,
		beam3Addr,
		beam4Addr,
		beam5Addr,
		beam6Addr,
		beam7Addr,
		beam8Addr,
		beam9Addr,
		NO_MODBUS_ADDR,
		0
	);
}

FactoryIO::lightArray_t FactoryIO::lightArray_t::constructAnalog(ModbusProvider_t& mb, modbusAddr_t analogInput, uint16_t scaleFactor) {
	return lightArray_t(
		mb,
		lightArrayMode_t::ANALOG,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		NO_MODBUS_ADDR,
		analogInput, 
		scaleFactor
	);
}

uint16_t FactoryIO::lightArray_t::getBitfieled() {
	uint16_t bitfield = 0;
	std::vector<bool> bitfieldArray;
	bool beamState = false;
	switch (_mode) {
	case FactoryIO::lightArrayMode_t::NUMERICAL:
		internal::checkModbusAddr(_valueAddr);
		return _mb.readInputRegister(_valueAddr);

	case FactoryIO::lightArrayMode_t::DIGITAL:
		internal::checkModbusAddr(_beam1Addr);
		internal::checkModbusAddr(_beam2Addr);
		internal::checkModbusAddr(_beam3Addr);
		internal::checkModbusAddr(_beam4Addr);
		internal::checkModbusAddr(_beam5Addr);
		internal::checkModbusAddr(_beam6Addr);
		internal::checkModbusAddr(_beam7Addr);
		internal::checkModbusAddr(_beam8Addr);
		internal::checkModbusAddr(_beam9Addr);
		bitfieldArray.push_back(_mb.readInputBit(_beam1Addr));
		bitfieldArray.push_back(_mb.readInputBit(_beam2Addr));
		bitfieldArray.push_back(_mb.readInputBit(_beam3Addr));
		bitfieldArray.push_back(_mb.readInputBit(_beam4Addr));
		bitfieldArray.push_back(_mb.readInputBit(_beam5Addr));
		bitfieldArray.push_back(_mb.readInputBit(_beam6Addr));
		bitfieldArray.push_back(_mb.readInputBit(_beam7Addr));
		bitfieldArray.push_back(_mb.readInputBit(_beam8Addr));
		bitfieldArray.push_back(_mb.readInputBit(_beam9Addr));
		return internal::BitfieldEnumMapper_t::toBitfield(bitfieldArray);
	case FactoryIO::lightArrayMode_t::ANALOG:
		throw std::domain_error("unable to get bitfield with ananlog value");
	default:
		throw std::runtime_error("unreachable state");
	}
}

bool FactoryIO::lightArray_t::isBeamInterupted() {
	if (_mode != lightArrayMode_t::ANALOG)
		return getBitfieled() != 0;


	internal::checkModbusAddr(_analogInput);
	return _mb.readInputRegister(_analogInput) != 0;
}

bool FactoryIO::lightArray_t::isBeamInterupted(uint16_t beamIndex) {
	if (_mode == lightArrayMode_t::ANALOG)
		throw std::domain_error("unable to get specific values with analog");
	return getBitfieled() >> beamIndex & 0x0001;
}

uint16_t FactoryIO::lightArray_t::getNumberOfBeamsInterupted() {

	if (_mode == lightArrayMode_t::ANALOG) {
		internal::checkModbusAddr(_analogInput);
		uint16_t analogValue = _mb.readInputRegister(_analogInput);
		
		if (analogValue == 0)
			return 0;
		return static_cast<uint16_t>(10 * (static_cast<float>(analogValue) / static_cast<float>(_scaleFactor)) / 8);
	}

	std::vector<bool> bitfieldArray = internal::BitfieldEnumMapper_t::toBool(getBitfieled());
	uint16_t numberOfBeamsInterrupted = 0;
	for (size_t i = 0; i < bitfieldArray.size(); i++) {
		if (bitfieldArray.at(i))
			numberOfBeamsInterrupted++;
	}

	return numberOfBeamsInterrupted;
}
