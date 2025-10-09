/*
* This file provides functionality that is used allover the FactoryIOLib
*/
#pragma once
#include "modbus.h"
#include <optional>

namespace FactoryIO {
	using modbusAddr_t = uint16_t;
	inline const modbusAddr_t NO_MODBUS_ADDR = -1;

	namespace internal {
		void checkModbusAddr(modbusAddr_t addr);
	}
}
