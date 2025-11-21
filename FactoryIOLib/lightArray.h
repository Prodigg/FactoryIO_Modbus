#pragma once
#include "FactoryIOGeneral.h"

namespace FactoryIO {
	enum class lightArrayMode_t  {
		NUMERICAL = 0,
		DIGITAL,
		ANALOG
	};

	class lightArray_t {
	public:
		lightArray_t(
			modbus& mb,
			lightArrayMode_t mode,
			modbusAddr_t valueAddr,
			modbusAddr_t beam1Addr,
			modbusAddr_t beam2Addr,
			modbusAddr_t beam3Addr,
			modbusAddr_t beam4Addr,
			modbusAddr_t beam5Addr,
			modbusAddr_t beam6Addr,
			modbusAddr_t beam7Addr,
			modbusAddr_t beam8Addr,
			modbusAddr_t beam9Addr,
			modbusAddr_t analogInput,
			uint16_t scaleFactor);

		static lightArray_t constructNumerical(modbus& mb, modbusAddr_t valueAddr);
		static lightArray_t constructDigital(
			modbus& mb,
			modbusAddr_t beam1Addr,
			modbusAddr_t beam2Addr,
			modbusAddr_t beam3Addr,
			modbusAddr_t beam4Addr,
			modbusAddr_t beam5Addr,
			modbusAddr_t beam6Addr,
			modbusAddr_t beam7Addr,
			modbusAddr_t beam8Addr,
			modbusAddr_t beam9Addr);
		static lightArray_t constructAnalog(modbus& mb, modbusAddr_t analogInput, uint16_t scaleFactor);

		uint16_t getBitfieled();
		bool isBeamInterupted();
		bool isBeamInterupted(uint16_t beamIndex);
		uint16_t getNumberOfBeamsInterupted();

	private:
		modbus& _mb;
		lightArrayMode_t _mode;
		modbusAddr_t _valueAddr;
		modbusAddr_t _beam1Addr;
		modbusAddr_t _beam2Addr;
		modbusAddr_t _beam3Addr;
		modbusAddr_t _beam4Addr;
		modbusAddr_t _beam5Addr;
		modbusAddr_t _beam6Addr;
		modbusAddr_t _beam7Addr;
		modbusAddr_t _beam8Addr;
		modbusAddr_t _beam9Addr;
		modbusAddr_t _analogInput;
		uint16_t _scaleFactor;
	};
}