#include "WarningLight.h"

void FactoryIO::WarningLight_t::setLightState(bool state) {
	internal::checkModbusAddr(_lightOnIndex);
	_lightOn = state; 
	updateFactoryIO(); 
}

void FactoryIO::WarningLight_t::updateFactoryIO() {
	if (_lightOnIndex != NO_MODBUS_ADDR)
		_mb.writeCoil(_lightOnIndex, _lightOn);
}