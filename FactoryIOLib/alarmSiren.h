#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"

namespace FactoryIO {
	class alarmSiren_t {
	public:
		alarmSiren_t(ModbusProvider_t& mb, modbusAddr_t sireneOnIndex) : _sireneOnIndex(sireneOnIndex), _mb(mb) { }
		void setSireneState(bool state);
	private:
		void updateFactoryIO();

		ModbusProvider_t& _mb;
		modbusAddr_t _sireneOnIndex;
		bool _sireneOn = false;
	};
}
