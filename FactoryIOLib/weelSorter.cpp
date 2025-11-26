#include "weelSorter.h"

FactoryIO::weelSorter_t::weelSorter_t(ModbusProvider_t& mb, modbusAddr_t plusAddr, modbusAddr_t minusAddr, modbusAddr_t leftAddr, modbusAddr_t rightAddr) :
	_mb(mb),
	_plusAddr(plusAddr),
	_minusAddr(minusAddr),
	_leftAddr(leftAddr),
	_rightAddr(rightAddr) { 
	internal::checkModbusAddr(_plusAddr, "unable to construct weelsorter without plusAddr");
}

void FactoryIO::weelSorter_t::moveForward() {
	if (_plusAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_plusAddr, true);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_minusAddr, false);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_leftAddr, false);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_rightAddr, false);
}

void FactoryIO::weelSorter_t::moveLeft() {
	internal::checkModbusAddr(_leftAddr);
	if (_plusAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_plusAddr, true);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_minusAddr, false);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_leftAddr, true);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_rightAddr, false);
}

void FactoryIO::weelSorter_t::moveRight() {
	internal::checkModbusAddr(_rightAddr);
	if (_plusAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_plusAddr, true);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_minusAddr, false);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_leftAddr, false);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_rightAddr, true);
}

void FactoryIO::weelSorter_t::moveBackward() {
	internal::checkModbusAddr(_minusAddr);
	if (_plusAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_plusAddr, false);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_minusAddr, true);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_leftAddr, false);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_rightAddr, false);
}

void FactoryIO::weelSorter_t::stop() {
	if (_plusAddr != NO_MODBUS_ADDR) 
		_mb.writeCoil(_plusAddr, false);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_minusAddr, false);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_leftAddr, false);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.writeCoil(_rightAddr, false);
}