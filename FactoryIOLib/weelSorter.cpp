#include "weelSorter.h"

FactoryIO::weelSorter_t::weelSorter_t(modbus& mb, modbusAddr_t plusAddr, modbusAddr_t leftAddr, modbusAddr_t rightAddr) : 
	_mb(mb),
	_plusAddr(plusAddr),
	_leftAddr(leftAddr),
	_rightAddr(rightAddr) { 
	internal::checkModbusAddr(plusAddr, "unable to create weelSorter without plusAddr");
	internal::checkModbusAddr(leftAddr, "unable to create weelSorter without leftAddr");
	internal::checkModbusAddr(rightAddr, "unable to create weelSorter without rightAddr");
}

void FactoryIO::weelSorter_t::moveForward() {
	_mb.modbus_write_coil(_plusAddr, true);
	_mb.modbus_write_coil(_leftAddr, false);
	_mb.modbus_write_coil(_rightAddr, false);
}

void FactoryIO::weelSorter_t::moveLeft() {
	_mb.modbus_write_coil(_plusAddr, true);
	_mb.modbus_write_coil(_leftAddr, true);
	_mb.modbus_write_coil(_rightAddr, false);
}

void FactoryIO::weelSorter_t::moveRight() {
	_mb.modbus_write_coil(_plusAddr, true);
	_mb.modbus_write_coil(_leftAddr, false);
	_mb.modbus_write_coil(_rightAddr, true);
}

void FactoryIO::weelSorter_t::stop() {
	_mb.modbus_write_coil(_plusAddr, false);
	_mb.modbus_write_coil(_leftAddr, false);
	_mb.modbus_write_coil(_rightAddr, false);
}