#include "convayorGate.h"

FactoryIO::convayorGate_t::convayorGate_t(
	ModbusProvider_t& mb, 
	modbusAddr_t digital, 
	modbusAddr_t digitalPlus, 
	modbusAddr_t digitalMinus, 
	modbusAddr_t analog, 
	ConvayorMode_t mode, 
	uint16_t scaleFactor,
	modbusAddr_t opend) : 
	Convayor_t(mb, digital, digitalPlus, digitalMinus, analog, mode, scaleFactor), 
	_opendIndex(opend) { }

bool FactoryIO::convayorGate_t::isGateOpend() {
	FactoryIO::internal::checkModbusAddr(_opendIndex);
	return mb.readInputBit(_opendIndex);
}
