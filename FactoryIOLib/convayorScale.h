#pragma once
#include "FactoryIOGeneral.h"
#include "convayor.h"

namespace FactoryIO {
	class convayorScale_t : private Convayor_t {
	public:
		convayorScale_t(modbus mb, modbusAddr_t digitalPlus, modbusAddr_t digitalMinus, modbusAddr_t currentWeight, uint16_t scaleFactor);
		
		using Convayor_t::moveDirection;
		using Convayor_t::move;
		using Convayor_t::stop;
		
		double getCurrentWeight();

	private:
		modbusAddr_t _currentWeightIndex;
	};
}