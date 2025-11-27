#pragma once
#include "FactoryIO.h"
#include "modbusProvider.h"
#include <array>
#include <type_traits>
#include <concepts>

namespace FactoryIO {
	class RFIDSensor_t {
	public:
		bool read(void* data, size_t size, byte offset);
		bool read(void* data, size_t size) {
			return read(data, size, 0);
		}

		template<typename T>
		bool read(T& data, size_t size = sizeof(T)) requires std::is_trivially_copyable<T>::value {
			void* dataPtr = &data;
			return read(dataPtr, size);
		}

		
		bool write(void* data, size_t size, byte offset);
		bool write(void* data, size_t size) {
			return write(data, size, 0);
		}

		template<typename T>
		bool write(T& data, size_t size = sizeof(T)) requires std::is_trivially_copyable<T>::value {
			void* dataPtr = &data;
			return write(dataPtr, size);
		}
		template<typename T>
		bool write(T&& data, size_t size = sizeof(T)) requires std::is_trivially_copyable<T>::value {
			void* dataPtr = &data;
			return write(dataPtr, size);
		}

	private:
		bool stateMachine();

		std::array<uint16_t, 128> _readBuffer{0};
		std::array<uint16_t, 128> _writeBuffer{0};

		enum class state_t {
			IDLE,

			READ_START,
			READ_REQUEST,
			READ_WAIT_RESPONSE,
			READ_END,

			WRITE_START,
			WRITE_DATA,
			WRITE_WAIT_RESPONSE,
			WRITE_END,

			CLEAR, 
			CLEAR_WAIT_RESPONSE,

			READ_SERIAL_NUMBER,
			READ_SERIAL_NUMBER_WAIT_RESPONSE,

			CHECK_IS_TAG_AVALABLE,
			CHECK_IS_TAG_AVALABLE_WAIT_RESPONSE
		} _state;
		
		enum class RFIDSensorCommand_t : uint16_t {
			IS_TAG_AVALABLE = 0, 
			READ_SERIAL_NUMBER,
			READ_DATA,
			WRITE_DATA, 
			CLEAR_ALL_DATA
		};

		enum class RFIDSensorStatus_t : uint16_t {
			SUCCESS = 0, 
			ERROR_NO_TAG,
			ERROR_TOO_MANY_TAGS,
			ERROR_INVALID_MEMORY_INDEX,
			INVALID_COMMAND
		};

		byte _currentMemoryAddress = 0;

		uint16_t _currentCommandID = 0;
	};
}