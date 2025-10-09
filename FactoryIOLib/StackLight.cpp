#include "StackLight.h"

FactoryIO::StackLight_t::StackLight_t(modbus& mb, modbusAddr_t lightGreenOnAddr, modbusAddr_t lightOrangeOnAddr, modbusAddr_t lightRedOnAddr) :
	_mb(mb),
	_lightGreenOnAddr(lightGreenOnAddr),
	_lightOrangeOnAddr(lightOrangeOnAddr),
	_lightRedOnAddr(lightRedOnAddr) {
}

void FactoryIO::StackLight_t::setGreenLight(bool state) {
	internal::checkModbusAddr(_lightGreenOnAddr);
	_lightGreenOn = state; 
	updateFactoryIO();
}

void FactoryIO::StackLight_t::setOrangeLight(bool state) {
	internal::checkModbusAddr(_lightOrangeOnAddr);
	_lightOrangeOn = state; 
	updateFactoryIO();
}

void FactoryIO::StackLight_t::setRedLight(bool state) {
	internal::checkModbusAddr(_lightRedOnAddr);
	_lightRedOn = state; 
	updateFactoryIO();
}

void FactoryIO::StackLight_t::updateFactoryIO() {
	if (_lightGreenOnAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_lightGreenOnAddr, _lightGreenOn);
	if (_lightOrangeOnAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_lightOrangeOnAddr, _lightOrangeOn);
	if (_lightRedOnAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_lightRedOnAddr, _lightRedOn);
}