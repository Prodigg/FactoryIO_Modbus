#pragma once
#include "FactoryIOGeneral.h"

namespace FactoryIO {
	class positioningBar_t {
	public:
		positioningBar_t(modbus& mb, modbusAddr_t clampAddr, modbusAddr_t raiseAddr, modbusAddr_t clampedLimitAddr, modbusAddr_t verticalLimitAddr);
		void clamp();
		void release();
		void raise();
		void lower();

		bool limitVerticalReached();
		bool isClamped();
	private:
		modbusAddr_t _clampAddr;
		modbusAddr_t _raiseAddr;
		modbusAddr_t _clampedLimitAddr;
		modbusAddr_t _verticalLimitAddr;
		modbus& _mb;
	};
}