#include "stopBlade.h"

void FactoryIO::stopBlade_t::setStopBladeState(bool state) {
	internal::checkModbusAddr(_stopBladeActiveAddr);
	_mb.modbus_write_coil(_stopBladeActiveAddr, state);
}
