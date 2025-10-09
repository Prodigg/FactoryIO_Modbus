#include "FactoryIOGeneral.h"
#include <stdexcept>

void FactoryIO::internal::checkModbusAddr(modbusAddr_t addr) {
	if (addr == NO_MODBUS_ADDR) {
		throw std::runtime_error("feature disabled due to providing no modbus address.");
	}
}
