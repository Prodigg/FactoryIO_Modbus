#pragma once
#include "FactoryIOGeneral.h"

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
		vision_t(modbus& mb,
			visionMode_t mode,
			modbusAddr_t singleDetectionAddr,
			modbusAddr_t bit0Addr,
			modbusAddr_t bit1Addr,
			modbusAddr_t bit2Addr,
			modbusAddr_t bit3Addr,
			modbusAddr_t bitfieldAddr,
			modbusAddr_t IDAddr);

		static vision_t constructDetectBase(modbus& mb, modbusAddr_t singleDetectionAddr);
		static vision_t constructDetectLid(modbus& mb, modbusAddr_t singleDetectionAddr);
		static vision_t constructDetectRaw(modbus& mb, modbusAddr_t singleDetectionAddr);
		static vision_t constructDetectAllDigital(
			modbus& mb, 
			modbusAddr_t bit0Addr,
			modbusAddr_t bit1Addr,
			modbusAddr_t bit2Addr,
			modbusAddr_t bit3Addr);
		static vision_t constructDetectNumerical(modbus& mb, modbusAddr_t bitfieldAddr);
		static vision_t constructDetectID(modbus& mb, modbusAddr_t IDAddr);

		bool isPartPresent();
		bool isLidPresent();
		bool isBasePresent();
		bool isRawPresent();

		uint16_t getPartID();
		Parts_t getDetectedPart();

	private:
		modbus& _mb;
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
	};
}