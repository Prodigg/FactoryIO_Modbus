#include "Emmiter.h"

FactoryIO::emmiter_t::emmiter_t(modbus& mb, modbusAddr_t emmitAddr, modbusAddr_t partAddr, modbusAddr_t baseAddr) : 
	_mb(mb), 
	_emmitAddr(emmitAddr), 
	_partAddr(partAddr), 
	_baseAddr(baseAddr) { }

void FactoryIO::emmiter_t::emmit(bool state) {
	internal::checkModbusAddr(_emmitAddr);
	_emmit = state;
	updateFactoryIO();
}

void FactoryIO::emmiter_t::setParts(std::vector<Parts_t> partsToEmmit) {
	FactoryIO::internal::checkModbusAddr(_partAddr);

	_part = 0; 
	for (FactoryIO::Parts_t& part : partsToEmmit) {
		switch (part) {
		case FactoryIO::Parts_t::NO_PART:
			break;
		case FactoryIO::Parts_t::SMALL_BOX:
			_part = _part | 1 << 0;
			break;
		case FactoryIO::Parts_t::MEDIUM_BOX:
			_part = _part | 1 << 1;
			break;
		case FactoryIO::Parts_t::LARGE_BOX:
			_part = _part | 1 << 2;
			break;
		case FactoryIO::Parts_t::PALLETIZING_BOX:
			_part = _part | 1 << 3;
			break;
		case FactoryIO::Parts_t::BLUE_RAW_MATERIAL:
			_part = _part | 1 << 4;
			break;
		case FactoryIO::Parts_t::GREEN_RAW_MATERIAL:
			_part = _part | 1 << 5;
			break;
		case FactoryIO::Parts_t::METTAL_RAW_MATERIAL:
			_part = _part | 1 << 6;
			break;
		case FactoryIO::Parts_t::BLUE_PRODUCT_BASE:
			_part = _part | 1 << 7;
			break;
		case FactoryIO::Parts_t::GREEN_PRODUCT_BASE:
			_part = _part | 1 << 8;
			break;
		case FactoryIO::Parts_t::METTAL_PRODUCT_BASE:
			_part = _part | 1 << 9;
			break;
		case FactoryIO::Parts_t::BLUE_PRODUCT_LID:
			_part = _part | 1 << 10;
			break;
		case FactoryIO::Parts_t::GREEN_PRODUCT_LID:
			_part = _part | 1 << 11;
			break;
		case FactoryIO::Parts_t::METTAL_PRODUCT_LID:
			_part = _part | 1 << 12;
			break;
		case FactoryIO::Parts_t::STACKABLE_BOX:
			_part = _part | 1 << 13;
			break;
		default:
			break;
		}
	}
	updateFactoryIO();
}

void FactoryIO::emmiter_t::setBase(std::vector<Bases_t> basesToEmmit) {
	FactoryIO::internal::checkModbusAddr(_baseAddr);

	_base = 0; 
	for (FactoryIO::Bases_t& base : basesToEmmit) {
		switch (base) {
		case FactoryIO::Bases_t::NO_BASE:
			break;
		case FactoryIO::Bases_t::PALLET:
			_base = _base | 1 << 0;
			break;
		case FactoryIO::Bases_t::SQARE_PALLET:
			_base = _base | 1 << 1;
			break;
		case FactoryIO::Bases_t::STACKABLE_BOX:
			_base = _base | 1 << 2;
			break;
		default:
			break;
		}
	}

	updateFactoryIO();
}

void FactoryIO::emmiter_t::updateFactoryIO() {
	if (_emmitAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_emmitAddr, _emmit);
	if (_partAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_register(_partAddr, _part);
	if (_baseAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_register(_baseAddr, _base);
}