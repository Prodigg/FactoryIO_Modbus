#include "rollerStop.h"

void FactoryIO::rollerStop_t::setRollerState(bool state) {
	FactoryIO::internal::checkModbusAddr(_rollerStopActiveAddr);
	_mb.modbus_write_coil(_rollerStopActiveAddr, state);
}
