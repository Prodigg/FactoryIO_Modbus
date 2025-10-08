#pragma once
#include "FactoryIOGeneral.h"
namespace FactoryIO {
	class alarmSiren_t {
	public:
		alarmSiren_t(modbus& mb, modbusAddr_t sireneOnIndex) : _sireneOnIndex(sireneOnIndex), _mb(mb) { }
		void setSireneState(bool state) { _sireneOn = state; updateFactoryIO(); }
	private:
		void updateFactoryIO() {
			_mb.modbus_write_coil(_sireneOnIndex, _sireneOn);
		}

		modbus& _mb; 
		modbusAddr_t _sireneOnIndex;
		bool _sireneOn = false;
	};
}
