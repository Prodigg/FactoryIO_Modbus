#pragma once
#include "FactoryIOGeneral.h"

namespace FactoryIO {
	class remover_t {
	public:
		remover_t(modbus& _mb, modbusAddr_t partsToRemove, modbusAddr_t basesToRemove, modbusAddr_t detectedParts, modbusAddr_t detectedBases);
		void setPartsToRemove(std::vector<FactoryIO::Parts_t> parts);
		void setBasesToRemove(std::vector<FactoryIO::Bases_t> bases);
		
		std::vector<FactoryIO::Parts_t> getDetectedParts();
		std::vector<FactoryIO::Bases_t> getDetectedBases();

	private:
		modbus& _mb;

		modbusAddr_t _partsToRemoveAddr;
		modbusAddr_t _basesToRemoveAddr;
		modbusAddr_t _detectedPartsAddr;
		modbusAddr_t _detectedBasesAddr;

		uint16_t _partsToRemove = 0;
		uint16_t _basesToRemove = 0;
		uint16_t _detectedParts = 0;
		uint16_t _detectedBases = 0;
	};
}
