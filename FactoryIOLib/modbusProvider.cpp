#include "modbusProvider.h"
#include <stdexcept>

FactoryIO::ModbusProvider_t::ModbusProvider_t(std::string host, uint16_t port, uint8_t slaveID) :
	_mb(host, port) { 
	_mb.modbus_set_slave_id(slaveID);
	_mb.modbus_connect();
}

FactoryIO::ModbusProvider_t::~ModbusProvider_t() {
	if (_inputBitsBuffer != nullptr)
		delete[] _inputBitsBuffer;
	if (_inputRegistersBuffer != nullptr)
		delete[] _inputRegistersBuffer;

	if (_mb.is_connected()) {
		_mb.modbus_close(); 
	}
}

void FactoryIO::ModbusProvider_t::writeHoldingRegister(modbusAddr_t addr, uint16_t value) {
	if (addr > _maxAllowedAddr)
		throw std::out_of_range("address is outside of allowed range");
	if (!_mb.is_connected())
		throw std::runtime_error("Modbus isn't connected, write not possible");

	if (_holdingRegistersBuffer.size() <= addr) {
		_holdingRegistersBuffer.resize(addr + 1, false);

		_mb.modbus_write_register(addr, value);
		_holdingRegistersBuffer.at(addr) = value;
		return;
	}

	if (_holdingRegistersBuffer.at(addr) != value) {
		_mb.modbus_write_register(addr, value);
		_holdingRegistersBuffer.at(addr) = value;
	}
}

void FactoryIO::ModbusProvider_t::writeCoil(modbusAddr_t addr, bool value) {
	if (addr > _maxAllowedAddr)
		throw std::out_of_range("address is outside of allowed range");
	if (!_mb.is_connected())
		throw std::runtime_error("Modbus isn't connected, write not possible");

	if (_coilsBuffer.size() <= addr) {
		_coilsBuffer.resize(addr + 1, false);

		_mb.modbus_write_coil(addr, value);
		_coilsBuffer.at(addr) = value;
		return;
	}
	
	if (_coilsBuffer.at(addr) != value) {
		_mb.modbus_write_coil(addr, value);
		_coilsBuffer.at(addr) = value;
	}
}

uint16_t FactoryIO::ModbusProvider_t::readInputRegister(modbusAddr_t addr) {
	if (addr > _maxAllowedAddr)
		throw std::out_of_range("address is outside of allowed range");

	if (_inputRegistersBufferSize <= addr) {
		// scary memory management
		uint16_t* newBuffer = new uint16_t[addr + 1];
		memset(newBuffer, 0, (static_cast<size_t>(addr) + 1) * sizeof(uint16_t));
		if (_inputRegistersBuffer != nullptr)
			memcpy(newBuffer, _inputRegistersBuffer, _inputRegistersBufferSize * sizeof(uint16_t));
		delete[] _inputRegistersBuffer;

		_inputRegistersBuffer = newBuffer;
		_inputRegistersBufferSize = addr + 1;

		forceUpdate();
		return _inputRegistersBuffer[addr];
	}

	updateInputValues();
	return _inputRegistersBuffer[addr];
}

bool FactoryIO::ModbusProvider_t::readInputBit(modbusAddr_t addr) {
	if (addr > _maxAllowedAddr)
		throw std::out_of_range("address is outside of allowed range");

	if (_inputBitsBufferSize <= addr) {
		// scary memory management
		bool* newBuffer = new bool[addr + 1];
		memset(newBuffer, 0, (static_cast<size_t>(addr) + 1) * sizeof(bool));
		if (_inputBitsBuffer != nullptr)
			memcpy(newBuffer, _inputBitsBuffer, _inputBitsBufferSize * sizeof(bool));
		delete[] _inputBitsBuffer;

		_inputBitsBuffer = newBuffer;
		_inputBitsBufferSize = addr + 1;

		forceUpdate();
		return _inputBitsBuffer[addr];
	}

	updateInputValues();
	return _inputBitsBuffer[addr];
}

void FactoryIO::ModbusProvider_t::forceUpdate() {
	_lastInputBitsRead = {};
	_lastInputRegistersRead = {};
	updateInputValues();
}

void FactoryIO::ModbusProvider_t::updateInputValues() {
	if (!_mb.is_connected())
		throw std::runtime_error("Modbus isn't connected, read not possible");

	if (std::chrono::steady_clock::now() - _lastInputBitsRead >= _inputBitsReadInterval) {
		_mb.modbus_read_input_bits(0, _inputBitsBufferSize, _inputBitsBuffer);
		_lastInputBitsRead = std::chrono::steady_clock::now();
	}

	if (std::chrono::steady_clock::now() - _lastInputRegistersRead >= _inputRegistersReadInterval) {
		_mb.modbus_read_input_registers(0, _inputRegistersBufferSize, _inputRegistersBuffer);
		_lastInputRegistersRead = std::chrono::steady_clock::now();
	}
}

