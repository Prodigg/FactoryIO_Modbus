#include "pivotArmSorter.h"

FactoryIO::pivotArmSorter_t::pivotArmSorter_t(ModbusProvider_t& mb, modbusAddr_t plusAddr, modbusAddr_t minusAddr, modbusAddr_t turnAddr) :
	Convayor_t(mb, NO_MODBUS_ADDR, plusAddr, minusAddr, NO_MODBUS_ADDR, ConvayorMode_t::DIGITAL_PLUS_MINUS, 1),
	_turnAddr(turnAddr) {}

void FactoryIO::pivotArmSorter_t::setArmPosition(bool extended) {
	internal::checkModbusAddr(_turnAddr);
	mb.writeCoil(_turnAddr, extended);
}
