#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"

namespace FactoryIO {
	class analogSensor_t {
	public:
		analogSensor_t(ModbusProvider_t& mb, modbusAddr_t sensorInputAddr, uint16_t scalingFactor) : _mb(mb), _sensorInputAddr(sensorInputAddr), _scalingFactor(scalingFactor) {};
		uint16_t getRawValue() {
			internal::checkModbusAddr(_sensorInputAddr);
			_mb.readInputRegister(_sensorInputAddr);
			return _mb.readInputRegister(_sensorInputAddr);
		};
	private:
		uint16_t _scalingFactor;
		modbusAddr_t _sensorInputAddr;
		ModbusProvider_t& _mb;
	};
}