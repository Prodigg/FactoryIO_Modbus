#pragma once
#include "FactoryIOGeneral.h"

namespace FactoryIO {
	class analogSensor_t {
	public:
		analogSensor_t(modbus& mb, modbusAddr_t sensorInputAddr, uint16_t scalingFactor) : _mb(mb), _sensorInputAddr(sensorInputAddr), _scalingFactor(scalingFactor) {};
		uint16_t getRawValue() {
			internal::checkModbusAddr(_sensorInputAddr);
			uint16_t state = 0;
			_mb.modbus_read_input_registers(_sensorInputAddr, 1, &state);

			return state;
		};
	private:
		uint16_t _scalingFactor;
		modbusAddr_t _sensorInputAddr;
		modbus& _mb;
	};
}