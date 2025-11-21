#pragma once
#include "FactoryIOGeneral.h"
#include "convayor.h"

namespace FactoryIO {
	class pivotArmSorter_t : private Convayor_t {
	public: 
		pivotArmSorter_t(modbus& mb, modbusAddr_t plusAddr, modbusAddr_t minusAddr, modbusAddr_t turnAddr);
		using Convayor_t::move;
		using Convayor_t::moveDirection;
		using Convayor_t::stop;
		void setArmPosition(bool extended);
	private: 
		modbusAddr_t _turnAddr;
	};
}