#pragma once
#include "FactoryIOGeneral.h"

namespace FactoryIO {
	class weelSorter_t {
	public:
		weelSorter_t(modbus& mb, modbusAddr_t plusAddr, modbusAddr_t leftAddr, modbusAddr_t rightAddr);
		void moveForward();
		void moveLeft();
		void moveRight();
		void stop();

	private:
		modbusAddr_t _plusAddr;
		modbusAddr_t _leftAddr;
		modbusAddr_t _rightAddr;
		modbus& _mb;
	};
}