#include "chainTransfer.h"

FactoryIO::chainTransfer_t::chainTransfer_t(modbus& mb, 
	modbusAddr_t chaintransferPlus, 
	modbusAddr_t chaintransferMinus, 
	modbusAddr_t chaintransferLeft, 
	modbusAddr_t chaintransferRight) : 
	_mb(mb), 
	_chaintransferPlusAddr(chaintransferPlus), 
	_chaintransferMinusAddr(chaintransferMinus), 
	_chaintransferLeftAddr(chaintransferLeft), 
	_chaintransferRightAddr(chaintransferRight) { }

void FactoryIO::chainTransfer_t::moveForward() {
	stop();
	FactoryIO::internal::checkModbusAddr(_chaintransferPlusAddr);
	_mb.modbus_write_coil(_chaintransferPlusAddr, true);
}

void FactoryIO::chainTransfer_t::moveBackward() {
	stop();
	FactoryIO::internal::checkModbusAddr(_chaintransferMinusAddr);
	_mb.modbus_write_coil(_chaintransferMinusAddr, true);
}

void FactoryIO::chainTransfer_t::moveLeft() {
	stop();
	FactoryIO::internal::checkModbusAddr(_chaintransferLeftAddr);
	_mb.modbus_write_coil(_chaintransferLeftAddr, true);
}

void FactoryIO::chainTransfer_t::moveRight() {
	stop();
	FactoryIO::internal::checkModbusAddr(_chaintransferRightAddr);
	_mb.modbus_write_coil(_chaintransferRightAddr, true);
}

void FactoryIO::chainTransfer_t::stop() {
	if (_chaintransferPlusAddr != FactoryIO::NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_chaintransferPlusAddr, false);

	if (_chaintransferMinusAddr != FactoryIO::NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_chaintransferMinusAddr, false);

	if (_chaintransferLeftAddr != FactoryIO::NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_chaintransferLeftAddr, false);

	if (_chaintransferRightAddr != FactoryIO::NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_chaintransferRightAddr, false);
}