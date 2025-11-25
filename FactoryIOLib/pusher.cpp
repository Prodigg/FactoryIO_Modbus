#include "pusher.h"
#include <stdexcept>

FactoryIO::pusher_t::pusher_t(
	modbus& mb,
	pusherMode_t mode,
	modbusAddr_t pusherAddr,
	modbusAddr_t frontLimitAddr,
	modbusAddr_t backLimitAddr,
	modbusAddr_t pusherPlusAddr,
	modbusAddr_t pusherMinusAddr,
	modbusAddr_t pusherSetpointAddr,
	modbusAddr_t pusherPositionAddr, 
	uint16_t scalefactor) :
	_mb(mb),
	_mode(mode),
	_pusherAddr(pusherAddr),
	_frontLimitAddr(frontLimitAddr),
	_backLimitAddr(backLimitAddr),
	_pusherPlusAddr(pusherPlusAddr),
	_pusherMinusAddr(pusherMinusAddr),
	_pusherSetpointAddr(pusherSetpointAddr),
	_pusherPositionAddr(pusherPositionAddr),
	_scaleFactor(scalefactor) {
	_pushSpeed = 5 * scalefactor;
}

FactoryIO::pusher_t FactoryIO::pusher_t::constructMonostable(modbus& mb, modbusAddr_t pusherAddr, modbusAddr_t frontLimitAddr, modbusAddr_t backLimitAddr) {
	return FactoryIO::pusher_t(mb, pusherMode_t::MONOSTABLE, pusherAddr, frontLimitAddr, backLimitAddr, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR, 0);
}

FactoryIO::pusher_t FactoryIO::pusher_t::constructBistable(modbus& mb, modbusAddr_t pusherPlusAddr, modbusAddr_t pusherMinusAddr, modbusAddr_t frontLimitAddr, modbusAddr_t backLimitAddr) {
	return pusher_t(mb, pusherMode_t::BISTABLE, NO_MODBUS_ADDR, frontLimitAddr, backLimitAddr, pusherPlusAddr, pusherMinusAddr, NO_MODBUS_ADDR, NO_MODBUS_ADDR, 0);
}

FactoryIO::pusher_t FactoryIO::pusher_t::constructAnalog(modbus& mb, modbusAddr_t pusherSetpointAddr, modbusAddr_t pusherPositionAddr, uint16_t scalefactor) {
	return pusher_t(mb, pusherMode_t::ANALOG, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR, pusherSetpointAddr, pusherPositionAddr, scalefactor);
}

FactoryIO::pusher_t FactoryIO::pusher_t::constructAnalogDigital(modbus& mb, modbusAddr_t pusherSetpointAddr, modbusAddr_t pusherPositionAddr, modbusAddr_t frontLimitAddr, modbusAddr_t backLimitAddr, uint16_t scalefactor) {
	return pusher_t(mb, pusherMode_t::DIGITAL_ANALOG, NO_MODBUS_ADDR, frontLimitAddr, backLimitAddr, NO_MODBUS_ADDR, NO_MODBUS_ADDR, pusherSetpointAddr, pusherPositionAddr, scalefactor);
}

void FactoryIO::pusher_t::push(bool forward) {
	_moveToTargetPos = false;
	switch (_mode) {
	case FactoryIO::pusherMode_t::MONOSTABLE:
		internal::checkModbusAddr(_pusherAddr);
		_mb.modbus_write_coil(_pusherAddr, forward);
		break;
	case FactoryIO::pusherMode_t::BISTABLE:
		internal::checkModbusAddr(_pusherPlusAddr);
		internal::checkModbusAddr(_pusherMinusAddr);
		_mb.modbus_write_coil(_pusherPlusAddr, forward);
		_mb.modbus_write_coil(_pusherMinusAddr, !forward);
		break;
	case FactoryIO::pusherMode_t::ANALOG:
		internal::checkModbusAddr(_pusherSetpointAddr);
		_mb.modbus_write_register(_pusherSetpointAddr, forward ? _pushSpeed : (-1) * _pushSpeed);
		break;
	case FactoryIO::pusherMode_t::DIGITAL_ANALOG:
		internal::checkModbusAddr(_pusherSetpointAddr);
		_mb.modbus_write_register(_pusherSetpointAddr, forward ? _pushSpeed : (-1) * _pushSpeed);
		break;
	default:
		throw std::runtime_error("unreachable state reached");
	}
}

bool FactoryIO::pusher_t::isAtFront() {
	bool frontLimit = false;
	uint16_t position = 0;
	switch (_mode) {
	case FactoryIO::pusherMode_t::MONOSTABLE:
		internal::checkModbusAddr(_frontLimitAddr);
		_mb.modbus_read_input_bits(_frontLimitAddr, 1, &frontLimit);
		return frontLimit;

	case FactoryIO::pusherMode_t::BISTABLE:
		internal::checkModbusAddr(_frontLimitAddr);
		_mb.modbus_read_input_bits(_frontLimitAddr, 1, &frontLimit);
		return frontLimit;

	case FactoryIO::pusherMode_t::ANALOG:
		internal::checkModbusAddr(_pusherPositionAddr);
		_mb.modbus_read_input_registers(_pusherPositionAddr, 1, &position);
		return position >= _scaleFactor * 10 - _acceptableOffDistance * _scaleFactor;

	case FactoryIO::pusherMode_t::DIGITAL_ANALOG:
		internal::checkModbusAddr(_frontLimitAddr);
		_mb.modbus_read_input_bits(_frontLimitAddr, 1, &frontLimit);
		return frontLimit;

	default:
		throw std::runtime_error("unrachable state reached");
	}
}

bool FactoryIO::pusher_t::isAtBack() {
	bool frontLimit = false;
	uint16_t position = 0;
	switch (_mode) {
	case FactoryIO::pusherMode_t::MONOSTABLE:
		internal::checkModbusAddr(_backLimitAddr);
		_mb.modbus_read_input_bits(_backLimitAddr, 1, &frontLimit);
		return frontLimit;

	case FactoryIO::pusherMode_t::BISTABLE:
		internal::checkModbusAddr(_backLimitAddr);
		_mb.modbus_read_input_bits(_backLimitAddr, 1, &frontLimit);
		return frontLimit;

	case FactoryIO::pusherMode_t::ANALOG:
		internal::checkModbusAddr(_pusherPositionAddr);
		_mb.modbus_read_input_registers(_pusherPositionAddr, 1, &position);
		return position <= _acceptableOffDistance * _scaleFactor;

	case FactoryIO::pusherMode_t::DIGITAL_ANALOG:
		internal::checkModbusAddr(_backLimitAddr);
		_mb.modbus_read_input_bits(_backLimitAddr, 1, &frontLimit);
		return frontLimit;

	default:
		throw std::runtime_error("unrachable state reached");
	}
}

float FactoryIO::pusher_t::getPosition() {
	if (_mode == pusherMode_t::MONOSTABLE || _mode == pusherMode_t::BISTABLE)
		throw std::domain_error("unable to provide position when not in analog mode");

	internal::checkModbusAddr(_pusherPositionAddr);
	uint16_t position = 0;
	_mb.modbus_read_input_registers(_pusherPositionAddr, 1, &position);
	return map<float>(position, 0.0f, 10.0f * _scaleFactor, 0.0f, 10.0f);
}

void FactoryIO::pusher_t::setPushSpeed(float speed) {
	if (speed > 1.0f || speed < 0.0f)
		throw std::out_of_range("speed out of range 1.0f - 0.0f");
	_pushSpeed = map<float>(speed, 0.0f, 1.0f, 0.0f, 10.0f * static_cast<float>(_scaleFactor));
}

bool FactoryIO::pusher_t::targetPosition(float targetPos) {
	if (_mode == pusherMode_t::MONOSTABLE || _mode == pusherMode_t::BISTABLE)
		throw std::domain_error("unable to move to a specific position when not in analog mode");
	internal::checkModbusAddr(_pusherPositionAddr);
	internal::checkModbusAddr(_pusherSetpointAddr);
	uint16_t positionRaw = 0;
	_mb.modbus_read_input_registers(_pusherPositionAddr, 1, &positionRaw);
	float position = map<float>(positionRaw, 0.0f, 10.0f * _scaleFactor, 0.0f, 1.0f);
	_moveToTargetPos = position > targetPos + _acceptableOffDistance || position < targetPos - _acceptableOffDistance;
	_targetPos = position;
	return _moveToTargetPos;
}

void FactoryIO::pusher_t::setSpeedFunction(std::function<float(float)> speedFunction) {
	_speedFunction = speedFunction;
}

void FactoryIO::pusher_t::setSpeedFunction(pusherSpeedFunction_t speedFunction) {
	switch (speedFunction) {
	case FactoryIO::pusherSpeedFunction_t::LINIAR:
		_speedFunction = [](float error) -> float {return error; };
		break;
	case FactoryIO::pusherSpeedFunction_t::EXPONENTIAL:
		_speedFunction = [](float error) -> float {return error * error; };
		break;
	default:
		throw std::runtime_error("unreachable state reached");
	}
}

void FactoryIO::pusher_t::update() {
	if (_mode == pusherMode_t::MONOSTABLE || _mode == pusherMode_t::BISTABLE)
		return;

	if (_moveToTargetPos) {
		uint16_t positionRaw = 0;
		_mb.modbus_read_input_registers(_pusherPositionAddr, 1, &positionRaw);
		float position = map<float>(positionRaw, 0.0f, 10.0f * static_cast<float>(_scaleFactor), 0.0f, 1.0f);

		if (position > _targetPos + _acceptableOffDistance || position < _targetPos - _acceptableOffDistance) {
			_moveToTargetPos = false;
			return;
		}

		// set speed
		float speed = 0.0f;
		speed = (*_speedFunction)(abs(_targetPos - position));
		if (_targetPos < position)
			speed *= -1.0f; // invert speed

		_mb.modbus_write_register(_pusherSetpointAddr, map<float>(speed, -1.0f, 1.0f, -10.0f * static_cast<float>(_scaleFactor), 10.0f * static_cast<float>(_scaleFactor)));
	}

}