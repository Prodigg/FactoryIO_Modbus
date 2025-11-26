#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"

namespace FactoryIO {
	class WarningLight_t {
	public:
		WarningLight_t(ModbusProvider_t& mb, modbusAddr_t sireneOnIndex) : _lightOnIndex(sireneOnIndex), _mb(mb) {}
		void setLightState(bool state);
	private:
		void updateFactoryIO();

		ModbusProvider_t& _mb;
		modbusAddr_t _lightOnIndex;
		bool _lightOn = false;
	};
}
