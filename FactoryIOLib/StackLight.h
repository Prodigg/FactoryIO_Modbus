#pragma once
#include "FactoryIOGeneral.h"
namespace FactoryIO {
	class StackLight_t {
	public:
		StackLight_t(modbus& mb, modbusAddr_t lightGreenOnAddr, modbusAddr_t lightOrangeOnAddr, modbusAddr_t lightRedOnAddr);
		void setGreenLight(bool state);
		void setOrangeLight(bool state);
		void setRedLight(bool state);
	private:
		void updateFactoryIO();

		modbus& _mb;
		modbusAddr_t _lightGreenOnAddr;
		modbusAddr_t _lightOrangeOnAddr;
		modbusAddr_t _lightRedOnAddr;

		bool _lightGreenOn = false;
		bool _lightOrangeOn = false;
		bool _lightRedOn = false;
	};
}
