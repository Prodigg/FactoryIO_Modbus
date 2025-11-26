#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"

namespace FactoryIO {
	enum class visionMode_t {
		DETECT_BASE = 0, 
		DETECT_LID,
		DETECT_RAW,
		ALL_DIGITAL,
		ALL_NUMERICAL,
		ALL_ID
	};
	
	class vision_t {
	public:
		vision_t(ModbusProvider_t& mb,
			visionMode_t mode,
			modbusAddr_t singleDetectionAddr,
			modbusAddr_t bit0Addr,
			modbusAddr_t bit1Addr,
			modbusAddr_t bit2Addr,
			modbusAddr_t bit3Addr,
			modbusAddr_t bitfieldAddr,
			modbusAddr_t IDAddr);

		static vision_t constructDetectBase(ModbusProvider_t& mb, modbusAddr_t singleDetectionAddr);
		static vision_t constructDetectLid(ModbusProvider_t& mb, modbusAddr_t singleDetectionAddr);
		static vision_t constructDetectRaw(ModbusProvider_t& mb, modbusAddr_t singleDetectionAddr);
		static vision_t constructDetectAllDigital(
			ModbusProvider_t& mb, 
			modbusAddr_t bit0Addr,
			modbusAddr_t bit1Addr,
			modbusAddr_t bit2Addr,
			modbusAddr_t bit3Addr);
		static vision_t constructDetectNumerical(ModbusProvider_t& mb, modbusAddr_t bitfieldAddr);
		static vision_t constructDetectID(ModbusProvider_t& mb, modbusAddr_t IDAddr);

		bool isPartPresent();
		bool isLidPresent();
		bool isBasePresent();
		bool isRawPresent();

		uint16_t getPartID();
		Parts_t getDetectedPart();

	private:
		ModbusProvider_t& _mb;
		visionMode_t _mode;

		modbusAddr_t _singleDetectionAddr;
		modbusAddr_t _bit0Addr;
		modbusAddr_t _bit1Addr;
		modbusAddr_t _bit2Addr;
		modbusAddr_t _bit3Addr;
		modbusAddr_t _bitfieldAddr;
		modbusAddr_t _IDAddr;

		Parts_t getPartDigital();
		Parts_t getPartNumerical();
		Parts_t getPartNumerical(uint16_t partNumber);
	};
}