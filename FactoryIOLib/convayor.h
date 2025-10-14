#pragma once
#include "FactoryIOGeneral.h"

namespace FactoryIO {

	enum class ConvayorMode_t {
		DIGITAL = 0, 
		DIGITAL_PLUS_MINUS, 
		ANALOG
	};

	class Convayor_t {
	public:
		Convayor_t(modbus& mb, modbusAddr_t digital, modbusAddr_t digitalPlus, modbusAddr_t digitalMinus, modbusAddr_t analog, ConvayorMode_t mode);

		void move(bool move);
		void moveDirection(bool reverse);
		void moveAtSpeed(float speed);
		void stop();

	private:
		modbus& mb;

		modbusAddr_t _digitalAddr;
		modbusAddr_t _digitalPlusAddr;
		modbusAddr_t _digitalMinusAddr;
		modbusAddr_t _analogAddr;

		bool _reverse = false;

		ConvayorMode_t _mode;
	};
}