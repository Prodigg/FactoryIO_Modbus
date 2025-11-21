#include "positioningBar.h"

FactoryIO::positioningBar_t::positioningBar_t(
	modbus& mb, 
	modbusAddr_t clampAddr, 
	modbusAddr_t raiseAddr, 
	modbusAddr_t clampedLimitAddr, 
	modbusAddr_t verticalLimitAddr) :
	_mb(mb),
	_clampAddr(clampAddr),
	_raiseAddr(raiseAddr),
	_clampedLimitAddr(clampedLimitAddr),
	_verticalLimitAddr(verticalLimitAddr) { }

void FactoryIO::positioningBar_t::clamp() {
	internal::checkModbusAddr(_clampAddr);
	_mb.modbus_write_coil(_clampAddr, true);
}
void FactoryIO::positioningBar_t::release() {
	internal::checkModbusAddr(_clampAddr);
	_mb.modbus_write_coil(_clampAddr, false);
}

void FactoryIO::positioningBar_t::raise() {
	internal::checkModbusAddr(_raiseAddr);
	_mb.modbus_write_coil(_raiseAddr, true);
}

void FactoryIO::positioningBar_t::lower() {
	internal::checkModbusAddr(_raiseAddr);
	_mb.modbus_write_coil(_raiseAddr, true);
}

bool FactoryIO::positioningBar_t::isRaised() {
	internal::checkModbusAddr(_verticalLimitAddr);
	bool isRaised = false;
	_mb.modbus_read_input_bits(_verticalLimitAddr, 1, &isRaised);
	return isRaised;
}

bool FactoryIO::positioningBar_t::isClamped() {
	internal::checkModbusAddr(_clampedLimitAddr);
	bool isClamped = false;
	_mb.modbus_read_input_bits(_clampedLimitAddr, 1, &isClamped);
	return isClamped;
}
