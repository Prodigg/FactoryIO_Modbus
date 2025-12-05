#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"

namespace FactoryIO {
	class digitalSensor_t {
	public:
		digitalSensor_t(ModbusProvider_t& mb, modbusAddr_t sensorInputAddr) : _mb(mb), _sensorInputAddr(sensorInputAddr) { };
		bool getState() {
			internal::checkModbusAddr(_sensorInputAddr);
			return _mb.readInputRegister(_sensorInputAddr);
		};
	private:
		modbusAddr_t _sensorInputAddr;
		ModbusProvider_t& _mb; 
	};
}