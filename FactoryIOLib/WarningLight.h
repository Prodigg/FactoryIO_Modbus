#pragma once
#include "FactoryIOGeneral.h"
namespace FactoryIO {
	class WarningLight_t {
	public:
		WarningLight_t(modbus& mb, modbusAddr_t sireneOnIndex) : _lightOnIndex(sireneOnIndex), _mb(mb) {}
		void setLightState(bool state) { _lightOn = state; updateFactoryIO(); }
	private:
		void updateFactoryIO() {
			_mb.modbus_write_coil(_lightOnIndex, _lightOn);
		}

		modbus& _mb;
		modbusAddr_t _lightOnIndex;
		bool _lightOn = false;
	};
}
