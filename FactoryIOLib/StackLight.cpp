#include "StackLight.h"

FactoryIO::StackLight_t::StackLight_t(modbus& mb, modbusAddr_t lightGreenOnAddr, modbusAddr_t lightOrangeOnAddr, modbusAddr_t lightRedOnAddr) :
	_mb(mb),
	_lightGreenOnAddr(lightGreenOnAddr),
	_lightOrangeOnAddr(lightOrangeOnAddr),
	_lightRedOnAddr(lightRedOnAddr) {
}

void FactoryIO::StackLight_t::updateFactoryIO() {
	_mb.modbus_write_coil(_lightGreenOnAddr, _lightGreenOn);
	_mb.modbus_write_coil(_lightOrangeOnAddr, _lightOrangeOn);
	_mb.modbus_write_coil(_lightRedOnAddr, _lightRedOn);
}