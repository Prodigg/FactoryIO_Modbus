#include "convayorGate.h"

FactoryIO::convayorGate_t::convayorGate_t(
	modbus& mb, 
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
	bool isConvayorOpen = false;
	mb.modbus_read_input_bits(_opendIndex, 1, &isConvayorOpen);
	return isConvayorOpen;
}
