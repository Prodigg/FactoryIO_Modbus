/*
* This file provides functionality that is used allover the FactoryIOLib
*/
#pragma once
#include "modbus.h"
#include <vector>


namespace FactoryIO {
	using modbusAddr_t = uint16_t;
	inline const modbusAddr_t NO_MODBUS_ADDR = -1;

	enum class Parts_t {
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

	enum class Bases_t {
		NO_BASE = 0,
		PALLET,
		SQARE_PALLET,
		STACKABLE_BOX
	};

	namespace internal {
		void checkModbusAddr(modbusAddr_t addr);

		/*!
		 * @brief converts standardised enums into bitfields
		 */
		class BitfieldEnumMapper_t {
		public:
			static uint16_t toBitfield(std::vector<Parts_t> _enum);
			static uint16_t toBitfield(std::vector<Bases_t> _enum);

			static std::vector<Parts_t> toParts(uint16_t bitfield);
			static std::vector<Bases_t> toBases(uint16_t bitfield);
		private:

		};
	}

	namespace presets {
		inline const std::vector<FactoryIO::Parts_t> allParts = {
		Parts_t::NO_PART,
		Parts_t::SMALL_BOX,
		Parts_t::MEDIUM_BOX,
		Parts_t::LARGE_BOX,
		Parts_t::PALLETIZING_BOX,
		Parts_t::BLUE_RAW_MATERIAL,
		Parts_t::GREEN_RAW_MATERIAL,
		Parts_t::METTAL_RAW_MATERIAL,
		Parts_t::BLUE_PRODUCT_BASE,
		Parts_t::GREEN_PRODUCT_BASE,
		Parts_t::METTAL_PRODUCT_BASE,
		Parts_t::BLUE_PRODUCT_LID,
		Parts_t::GREEN_PRODUCT_LID,
		Parts_t::METTAL_PRODUCT_LID,
		Parts_t::STACKABLE_BOX
		};

		inline const std::vector<FactoryIO::Bases_t> allBases = {
		Bases_t::NO_BASE,
		Bases_t::PALLET,
		Bases_t::SQARE_PALLET,
		Bases_t::STACKABLE_BOX
		};
	}


}
