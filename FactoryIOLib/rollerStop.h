#pragma once
#include "FactoryIOGeneral.h"
namespace FactoryIO {
	class rollerStop_t {
	public:
		rollerStop_t(modbus& mb, modbusAddr_t rollerStopIndex) : _rollerStopActiveAddr(rollerStopIndex), _mb(mb) {}
		void setRollerState(bool state);
	private:
		modbus& _mb;
		modbusAddr_t _rollerStopActiveAddr;
	};
}