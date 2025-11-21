#include "FactoryIOGeneral.h"
#include <stdexcept>

void FactoryIO::internal::checkModbusAddr(modbusAddr_t addr, std::string message) {
	if (addr == NO_MODBUS_ADDR) {
		throw std::runtime_error(message);
	}
}

uint16_t FactoryIO::internal::BitfieldEnumMapper_t::toBitfield(std::vector<FactoryIO::Parts_t> _enum) {
	uint16_t _part = 0;
	for (FactoryIO::Parts_t& part : _enum) {
		switch (part) {
		case FactoryIO::Parts_t::NO_PART:
			break;
		case FactoryIO::Parts_t::SMALL_BOX:
			_part = _part | 1 << 0;
			break;
		case FactoryIO::Parts_t::MEDIUM_BOX:
			_part = _part | 1 << 1;
			break;
		case FactoryIO::Parts_t::LARGE_BOX:
			_part = _part | 1 << 2;
			break;
		case FactoryIO::Parts_t::PALLETIZING_BOX:
			_part = _part | 1 << 3;
			break;
		case FactoryIO::Parts_t::BLUE_RAW_MATERIAL:
			_part = _part | 1 << 4;
			break;
		case FactoryIO::Parts_t::GREEN_RAW_MATERIAL:
			_part = _part | 1 << 5;
			break;
		case FactoryIO::Parts_t::METTAL_RAW_MATERIAL:
			_part = _part | 1 << 6;
			break;
		case FactoryIO::Parts_t::BLUE_PRODUCT_BASE:
			_part = _part | 1 << 7;
			break;
		case FactoryIO::Parts_t::GREEN_PRODUCT_BASE:
			_part = _part | 1 << 8;
			break;
		case FactoryIO::Parts_t::METTAL_PRODUCT_BASE:
			_part = _part | 1 << 9;
			break;
		case FactoryIO::Parts_t::BLUE_PRODUCT_LID:
			_part = _part | 1 << 10;
			break;
		case FactoryIO::Parts_t::GREEN_PRODUCT_LID:
			_part = _part | 1 << 11;
			break;
		case FactoryIO::Parts_t::METTAL_PRODUCT_LID:
			_part = _part | 1 << 12;
			break;
		case FactoryIO::Parts_t::STACKABLE_BOX:
			_part = _part | 1 << 13;
			break;
		default:
			throw std::runtime_error("unknown enum found, cannot convert to bitfield.");
		}
	}
	return _part;
}

uint16_t FactoryIO::internal::BitfieldEnumMapper_t::toBitfield(std::vector<FactoryIO::Bases_t> _enum) {
	uint16_t _base = 0;
	for (FactoryIO::Bases_t& base : _enum) {
		switch (base) {
		case FactoryIO::Bases_t::NO_BASE:
			break;
		case FactoryIO::Bases_t::PALLET:
			_base = _base | 1 << 0;
			break;
		case FactoryIO::Bases_t::SQARE_PALLET:
			_base = _base | 1 << 1;
			break;
		case FactoryIO::Bases_t::STACKABLE_BOX:
			_base = _base | 1 << 2;
			break;
		default:
			throw std::runtime_error("unknown enum found, cannot convert to bitfield.");
		}
	}
	return _base;
}

uint16_t FactoryIO::internal::BitfieldEnumMapper_t::toBitfield(std::vector<bool> _boolArray) {
	if (_boolArray.size() > 16)
		throw std::out_of_range("Array can't be larger than 16");
	uint16_t bitfield = 0;
	for (size_t i = 0; i < _boolArray.size(); i++) {
		bitfield |= _boolArray.at(i) << i;
	}
	return bitfield;
}

std::vector<FactoryIO::Parts_t> FactoryIO::internal::BitfieldEnumMapper_t::toParts(uint16_t bitfield) {
	std::vector<FactoryIO::Parts_t> returnParts{};
	returnParts.reserve(16);
	if ((bitfield >> 0 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::SMALL_BOX);
	if ((bitfield >> 1 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::MEDIUM_BOX);
	if ((bitfield >> 2 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::LARGE_BOX);
	if ((bitfield >> 3 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::PALLETIZING_BOX);
	if ((bitfield >> 4 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::BLUE_RAW_MATERIAL);
	if ((bitfield >> 5 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::GREEN_RAW_MATERIAL);
	if ((bitfield >> 6 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::METTAL_RAW_MATERIAL);
	if ((bitfield >> 7 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::BLUE_PRODUCT_BASE);
	if ((bitfield >> 8 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::GREEN_PRODUCT_BASE);
	if ((bitfield >> 9 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::METTAL_PRODUCT_BASE);
	if ((bitfield >> 10 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::BLUE_PRODUCT_LID);
	if ((bitfield >> 11 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::GREEN_PRODUCT_LID);
	if ((bitfield >> 12 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::METTAL_PRODUCT_LID);
	if ((bitfield >> 13 & 0x0001) == true)
		returnParts.push_back(FactoryIO::Parts_t::STACKABLE_BOX);
	if ((bitfield >> 14 & 0xFFFF)) // defualt case
		throw std::runtime_error("bitfield has values that cant be decodet.");

	returnParts.shrink_to_fit();
	return returnParts;
}

std::vector<FactoryIO::Bases_t> FactoryIO::internal::BitfieldEnumMapper_t::toBases(uint16_t bitfield) {
	std::vector<FactoryIO::Bases_t> returnBases{};
	returnBases.reserve(3);

	if ((bitfield >> 0 & 0x0001) == true)
		returnBases.push_back(FactoryIO::Bases_t::PALLET);
	if ((bitfield >> 1 & 0x0001) == true)
		returnBases.push_back(FactoryIO::Bases_t::SQARE_PALLET);
	if ((bitfield >> 2 & 0x0001) == true)
		returnBases.push_back(FactoryIO::Bases_t::STACKABLE_BOX);
	if ((bitfield >> 3 & 0xFFFF) ) // defualt case
		throw std::runtime_error("bitfield has values that cant be decodet.");

	returnBases.shrink_to_fit();
	return returnBases;
}

std::vector<bool> FactoryIO::internal::BitfieldEnumMapper_t::toBool(uint16_t bitfield, size_t maxRelevantBit) {
	std::vector<bool> bitfieldArray;
	for (size_t i = 0; i < maxRelevantBit; i++) {
		bitfieldArray.push_back(bitfield >> i & 0x0001);
	}
	return bitfieldArray;
}

bool FactoryIO::internal::testing::getModbusCoilState(FactoryIO::modbusAddr_t addr, modbus& mb) {
	bool tmp = false;
	mb.modbus_read_coils(addr, 1, &tmp);
	return tmp;
}

uint16_t FactoryIO::internal::testing::getModbusRegState(FactoryIO::modbusAddr_t addr, modbus& mb) {
	uint16_t tmp = 0;
	mb.modbus_read_holding_registers(addr, 1, &tmp);
	return tmp;
}

