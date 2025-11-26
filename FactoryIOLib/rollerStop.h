#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"
namespace FactoryIO {
	class rollerStop_t {
	public:
		rollerStop_t(ModbusProvider_t& mb, modbusAddr_t rollerStopIndex) : _rollerStopActiveAddr(rollerStopIndex), _mb(mb) {}
		void setRollerState(bool state);
	private:
		ModbusProvider_t& _mb;
		modbusAddr_t _rollerStopActiveAddr;
	};
}