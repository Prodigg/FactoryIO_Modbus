#pragma once
#include "FactoryIOGeneral.h"

namespace FactoryIO {
	class digitalSensor_t {
	public:
		digitalSensor_t(modbus& mb, modbusAddr_t sensorInputAddr) : _mb(mb), _sensorInputAddr(sensorInputAddr) { };
		bool getState() {
			internal::checkModbusAddr(_sensorInputAddr);
			bool state = false;
			_mb.modbus_read_input_bits(_sensorInputAddr, 1, &state);
			return state;
		};
	private:
		modbusAddr_t _sensorInputAddr;
		modbus& _mb; 
	};
}