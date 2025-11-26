#include "rollerStop.h"

void FactoryIO::rollerStop_t::setRollerState(bool state) {
	FactoryIO::internal::checkModbusAddr(_rollerStopActiveAddr);
	_mb.writeCoil(_rollerStopActiveAddr, state);
}
