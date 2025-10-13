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

void FactoryIO::emmiter_t::setParts(const std::vector<Parts_t> partsToEmmit) {
	FactoryIO::internal::checkModbusAddr(_partAddr);

	_part = 0; 
	_part = FactoryIO::internal::BitfieldEnumMapper_t::toBitfield(partsToEmmit);
	updateFactoryIO();
}

void FactoryIO::emmiter_t::setBase(const std::vector<Bases_t> basesToEmmit) {
	FactoryIO::internal::checkModbusAddr(_baseAddr);

	_base = 0;
	_base = FactoryIO::internal::BitfieldEnumMapper_t::toBitfield(basesToEmmit);

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