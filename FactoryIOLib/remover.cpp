#include "remover.h"

FactoryIO::remover_t::remover_t(modbus& _mb, modbusAddr_t partsToRemove, modbusAddr_t basesToRemove, modbusAddr_t detectedParts, modbusAddr_t detectedBases) :
	_mb(_mb), 
	_partsToRemoveAddr(partsToRemove), 
	_basesToRemoveAddr(basesToRemove), 
	_detectedPartsAddr(detectedParts), 
	_detectedBasesAddr(detectedBases) { }

void FactoryIO::remover_t::setPartsToRemove(std::vector<FactoryIO::Parts_t> parts) {
	internal::checkModbusAddr(_partsToRemoveAddr);
	_partsToRemove = internal::BitfieldEnumMapper_t::toBitfield(parts);
	
	_mb.modbus_write_register(_partsToRemoveAddr, _partsToRemove);
}

void FactoryIO::remover_t::setBasesToRemove(std::vector<FactoryIO::Bases_t> bases) {
	internal::checkModbusAddr(_basesToRemoveAddr);
	_basesToRemove = internal::BitfieldEnumMapper_t::toBitfield(bases);

	_mb.modbus_write_register(_basesToRemoveAddr, _basesToRemove);
}

std::vector<FactoryIO::Parts_t> FactoryIO::remover_t::getDetectedParts() {
	internal::checkModbusAddr(_detectedPartsAddr);

	_mb.modbus_read_input_registers(_detectedPartsAddr, 1, &_detectedParts);

	return internal::BitfieldEnumMapper_t::toParts(_detectedParts);
}

std::vector<FactoryIO::Bases_t> FactoryIO::remover_t::getDetectedBases() {
	internal::checkModbusAddr(_detectedPartsAddr);

	_mb.modbus_read_input_registers(_detectedBasesAddr, 1, &_detectedBases);

	return internal::BitfieldEnumMapper_t::toBases(_detectedBases);
}

