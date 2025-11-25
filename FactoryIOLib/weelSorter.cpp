#include "weelSorter.h"

FactoryIO::weelSorter_t::weelSorter_t(modbus& mb, modbusAddr_t plusAddr, modbusAddr_t minusAddr, modbusAddr_t leftAddr, modbusAddr_t rightAddr) :
	_mb(mb),
	_plusAddr(plusAddr),
	_minusAddr(minusAddr),
	_leftAddr(leftAddr),
	_rightAddr(rightAddr) { 
	internal::checkModbusAddr(_plusAddr, "unable to construct weelsorter without plusAddr");
}

void FactoryIO::weelSorter_t::moveForward() {
	if (_plusAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_plusAddr, true);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_minusAddr, false);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_leftAddr, false);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_rightAddr, false);
}

void FactoryIO::weelSorter_t::moveLeft() {
	internal::checkModbusAddr(_leftAddr);
	if (_plusAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_plusAddr, true);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_minusAddr, false);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_leftAddr, true);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_rightAddr, false);
}

void FactoryIO::weelSorter_t::moveRight() {
	internal::checkModbusAddr(_rightAddr);
	if (_plusAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_plusAddr, true);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_minusAddr, false);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_leftAddr, false);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_rightAddr, true);
}

void FactoryIO::weelSorter_t::moveBackward() {
	internal::checkModbusAddr(_minusAddr);
	if (_plusAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_plusAddr, false);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_minusAddr, true);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_leftAddr, false);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_rightAddr, false);
}

void FactoryIO::weelSorter_t::stop() {
	if (_plusAddr != NO_MODBUS_ADDR) 
		_mb.modbus_write_coil(_plusAddr, false);
	if (_minusAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_minusAddr, false);
	if (_leftAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_leftAddr, false);
	if (_rightAddr != NO_MODBUS_ADDR)
		_mb.modbus_write_coil(_rightAddr, false);
}