#pragma once
#include "FactoryIOGeneral.h"
#include <vector>

namespace FactoryIO {
	enum class PartToEmmit_t {
		NO_PART = 0,
		SMALL_BOX = 1, 
		MEDIUM_BOX = 2,
		LARGE_BOX = 3,
		PALLETIZING_BOX = 4,
		BLUE_RAW_MATERIAL = 5, 
		GREEN_RAW_MATERIAL = 6, 
		METTAL_RAW_MATERIAL = 7,
		BLUE_PRODUCT_BASE = 8,
		GREEN_PRODUCT_BASE = 9,
		METTAL_PRODUCT_BASE = 10, 
		BLUE_PRODUCT_LID = 11,
		GREEN_PRODUCT_LID = 12,
		METTAL_PRODUCT_LID = 13, 
		STACKABLE_BOX = 14
	};

	enum class baseToEmmit_t {
		NO_BASE = 0,
		PALLET, 
		SQARE_PALLET, 
		STACKABLE_BOX
	};

	class emmiter_t {
	public:
		emmiter_t(modbus& mb, modbusAddr_t emmitAddr, modbusAddr_t partAddr, modbusAddr_t baseAddr);
		void emmit(bool state);
		void setParts(std::vector<PartToEmmit_t> partsToEmmit);
		void setBase(std::vector<baseToEmmit_t> basesToEmmit);

	private:
		void updateFactoryIO();

		modbusAddr_t _emmitAddr; 
		modbusAddr_t _partAddr;
		modbusAddr_t _baseAddr;

		bool _emmit = false;
		int _part = 0;
		int _base = 0;
		modbus& _mb;
		
	};
}