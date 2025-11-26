#pragma once
#include "FactoryIOGeneral.h"
#include <chrono>

namespace FactoryIO {
	class ModbusProvider_t {
	public:
		ModbusProvider_t(std::string host, uint16_t port, uint8_t slaveID);
		~ModbusProvider_t();

		modbus& getModbus() { return _mb; }

		void writeHoldingRegister(modbusAddr_t addr, uint16_t value);
		void writeCoil(modbusAddr_t addr, bool value);

		uint16_t readInputRegister(modbusAddr_t addr);
		bool readInputBit(modbusAddr_t addr);

		inline void setInputBitsReadInterval(std::chrono::steady_clock::duration durration) { _inputBitsReadInterval = durration; }
		inline void setInputRegistersReadInterval(std::chrono::steady_clock::duration durration) { _inputRegistersReadInterval = durration; }

		void forceUpdate();

		inline void setMaxAllowedAddr(uint16_t maxAllowedAddr) { _maxAllowedAddr = maxAllowedAddr; }
		inline uint16_t getMaxAllowedAddr() const { return _maxAllowedAddr; }

	private:
		std::chrono::steady_clock::duration _inputBitsReadInterval = std::chrono::milliseconds(50);
		std::chrono::steady_clock::duration _inputRegistersReadInterval = std::chrono::milliseconds(50);

		std::chrono::steady_clock::time_point _lastInputBitsRead;
		std::chrono::steady_clock::time_point _lastInputRegistersRead;

		uint16_t _maxAllowedAddr = 1024; // limitor of max ammount of addresses to allocate

		uint16_t _inputBitsBufferSize = 0;
		bool* _inputBitsBuffer = nullptr;

		uint16_t _inputRegistersBufferSize = 0;
		uint16_t* _inputRegistersBuffer = nullptr;

		std::vector<bool> _coilsBuffer;
		std::vector<uint16_t> _holdingRegistersBuffer;

		modbus _mb;

		/*!
		 * @brief update logic for the buffers
		 */
		void updateInputValues();
	};
}
