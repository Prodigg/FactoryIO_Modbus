#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"
namespace FactoryIO {
	class stopBlade_t {
	public:
		stopBlade_t(ModbusProvider_t& mb, modbusAddr_t stopBladeIndex) : _stopBladeActiveAddr(stopBladeIndex), _mb(mb) {}
		void setStopBladeState(bool state);
	private:
		ModbusProvider_t& _mb;
		modbusAddr_t _stopBladeActiveAddr;
	};
}