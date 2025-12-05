#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"

namespace FactoryIO {
	class weelSorter_t {
	public:
		weelSorter_t(ModbusProvider_t& mb, modbusAddr_t plusAddr, modbusAddr_t minusAddr, modbusAddr_t leftAddr, modbusAddr_t rightAddr);
		void moveForward();
		void moveBackward();
		void moveLeft();
		void moveRight();
		void stop();

	private:
		modbusAddr_t _plusAddr;
		modbusAddr_t _minusAddr;
		modbusAddr_t _leftAddr;
		modbusAddr_t _rightAddr;
		ModbusProvider_t& _mb;
	};
}