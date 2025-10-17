#pragma once
#include "FactoryIOGeneral.h"
namespace FactoryIO {
	class chainTransfer_t {
	public:
		chainTransfer_t(modbus& mb, modbusAddr_t chaintransferPlus, modbusAddr_t chaintransferMinus, modbusAddr_t chaintransferLeft, modbusAddr_t chaintransferRight);
		void moveForward();
		void moveBackward();
		void moveLeft();
		void moveRight();
		void stop();
	private:
		modbus& _mb;
		modbusAddr_t _chaintransferPlusAddr;
		modbusAddr_t _chaintransferMinusAddr;
		modbusAddr_t _chaintransferLeftAddr;
		modbusAddr_t _chaintransferRightAddr;
	};
}