#include "RFIDSensor.h"
#include <stdexcept>
#include <cmath>

bool FactoryIO::RFIDSensor_t::read(void* data, size_t size, byte offset) {

	if (_state != state_t::IDLE && _state != state_t::READ_REQUEST && _state != state_t::READ_WAIT_RESPONSE && _state != state_t::READ_END)
		throw std::runtime_error("unable to read, state is not valid");
	if (_state == state_t::IDLE) {
		_currentMemoryAddress = offset;
		_memorySize = size;
		_state = state_t::READ_START;
	}

	stateMachine();

	if (_state == state_t::READ_END) {
		for (size_t i = 0; i < size;) {
			uint16_t singleValue = _readBuffer.at((i / 2) + offset);
			((uint8_t*)data)[i] = singleValue & 0x00FF;
			if (i + 1 < size)
				((uint8_t*)data)[i + 1] = singleValue >> 8;
			i = i + 2;
		}
		_state = state_t::IDLE;
		return true;
	}
	return false;
}

bool FactoryIO::RFIDSensor_t::write(void* data, size_t size, byte offset) {
	if (_state != state_t::IDLE && _state != state_t::WRITE_DATA && _state != state_t::WRITE_WAIT_RESPONSE && _state != state_t::WRITE_END)
		throw std::runtime_error("unable to write, state is not valid");
	if (_state == state_t::IDLE) {
		// start write
		for (size_t i = 0; i < size;) {
			uint16_t singleValue = 0;
			if (i + 1 < size)
				singleValue |= ((uint8_t*)data)[i + 1] << 8;
			singleValue |= ((uint8_t*)data)[i];
			_writeBuffer.at((i / 2) + offset) = singleValue;
			i = i + 2;
		}
		_memorySize = size;
		_currentMemoryAddress = offset;
		_state = state_t::WRITE_START;
	}

	return stateMachine();
}

bool FactoryIO::RFIDSensor_t::stateMachine() {
	switch (_state) {
	case FactoryIO::RFIDSensor_t::state_t::IDLE:
		break;
	case FactoryIO::RFIDSensor_t::state_t::READ_START:
		break;
	case FactoryIO::RFIDSensor_t::state_t::READ_REQUEST:
		break;
	case FactoryIO::RFIDSensor_t::state_t::READ_WAIT_RESPONSE:
		break;
	case FactoryIO::RFIDSensor_t::state_t::READ_END:
		break;
	case FactoryIO::RFIDSensor_t::state_t::WRITE_START:
		break;
	case FactoryIO::RFIDSensor_t::state_t::WRITE_DATA:
		break;
	case FactoryIO::RFIDSensor_t::state_t::WRITE_WAIT_RESPONSE:
		break;
	case FactoryIO::RFIDSensor_t::state_t::WRITE_END:
		break;
	case FactoryIO::RFIDSensor_t::state_t::CLEAR:
		break;
	case FactoryIO::RFIDSensor_t::state_t::CLEAR_WAIT_RESPONSE:
		break;
	case FactoryIO::RFIDSensor_t::state_t::READ_SERIAL_NUMBER:
		break;
	case FactoryIO::RFIDSensor_t::state_t::READ_SERIAL_NUMBER_WAIT_RESPONSE:
		break;
	case FactoryIO::RFIDSensor_t::state_t::CHECK_IS_TAG_AVALABLE:
		break;
	case FactoryIO::RFIDSensor_t::state_t::CHECK_IS_TAG_AVALABLE_WAIT_RESPONSE:
		break;
	default:
		break;
	}
	return false;
}
