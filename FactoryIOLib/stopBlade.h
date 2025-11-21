#pragma once
#include "FactoryIOGeneral.h"
namespace FactoryIO {
	class stopBlade_t {
	public:
		stopBlade_t(modbus& mb, modbusAddr_t stopBladeIndex) : _stopBladeActiveAddr(stopBladeIndex), _mb(mb) {}
		void setStopBladeState(bool state);
	private:
		modbus& _mb;
		modbusAddr_t _stopBladeActiveAddr;
	};
}