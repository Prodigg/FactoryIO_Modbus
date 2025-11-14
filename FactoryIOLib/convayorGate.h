#pragma once
#include "FactoryIOGeneral.h"
#include "convayor.h"

namespace FactoryIO {
	class convayorGate_t : public FactoryIO::Convayor_t {
	public:
		convayorGate_t(modbus& mb, modbusAddr_t digital, modbusAddr_t digitalPlus, modbusAddr_t digitalMinus, modbusAddr_t analog, ConvayorMode_t mode, uint16_t scaleFactor, modbusAddr_t opend);
		
		bool isGateOpend();

	private:
		modbusAddr_t _opendIndex;

	};
}
