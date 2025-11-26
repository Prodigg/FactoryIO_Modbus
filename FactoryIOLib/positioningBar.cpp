#include "positioningBar.h"

FactoryIO::positioningBar_t::positioningBar_t(
	ModbusProvider_t& mb, 
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
	_mb.writeCoil(_clampAddr, true);
}
void FactoryIO::positioningBar_t::release() {
	internal::checkModbusAddr(_clampAddr);
	_mb.writeCoil(_clampAddr, false);
}

void FactoryIO::positioningBar_t::raise() {
	internal::checkModbusAddr(_raiseAddr);
	_mb.writeCoil(_raiseAddr, true);
}

void FactoryIO::positioningBar_t::lower() {
	internal::checkModbusAddr(_raiseAddr);
	_mb.writeCoil(_raiseAddr, false);
}

bool FactoryIO::positioningBar_t::limitVerticalReached() {
	internal::checkModbusAddr(_verticalLimitAddr);
	return _mb.readInputBit(_verticalLimitAddr);
}

bool FactoryIO::positioningBar_t::isClamped() {
	internal::checkModbusAddr(_clampedLimitAddr);
	return _mb.readInputBit(_clampedLimitAddr);
}
