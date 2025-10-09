#include "alarmSiren.h"

void FactoryIO::alarmSiren_t::setSireneState(bool state) {
	internal::checkModbusAddr(_sireneOnIndex);
	_sireneOn = state; 
	updateFactoryIO();
}

void FactoryIO::alarmSiren_t::updateFactoryIO() {
	if (_sireneOnIndex != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_sireneOnIndex, _sireneOn);
}