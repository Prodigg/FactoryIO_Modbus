#pragma once
#include "FactoryIOGeneral.h"
namespace FactoryIO {
	class WarningLight_t {
	public:
		WarningLight_t(modbus& mb, modbusAddr_t sireneOnIndex) : _lightOnIndex(sireneOnIndex), _mb(mb) {}
		void setLightState(bool state);
	private:
		void updateFactoryIO();

		modbus& _mb;
		modbusAddr_t _lightOnIndex;
		bool _lightOn = false;
	};
}
