#include "turntable.h"
#include <iostream>
#include <stdexcept>

FactoryIO::turntable_t::turntable_t(
	modbus& mb,
	modbusAddr_t rollPlus,
	modbusAddr_t rollMinus,
	modbusAddr_t frontLimit,
	modbusAddr_t backLimit,
	modbusAddr_t limit0,
	modbusAddr_t limit90,
	turntableMode_t mode,
	modbusAddr_t turn,
	modbusAddr_t turnPlus,
	modbusAddr_t turnMinus) :
	_mb(mb),
	_rollPlusIndex(rollPlus), 
	_rollMinusIndex(rollMinus),
	_frontLimitIndex(frontLimit),
	_backLimitIndex(backLimit),
	_limit0Index(limit0),
	_limit90Index(limit90),
	_mode(mode),
	_turnIndex(turn),
	_turnPlusIndex(turnPlus),
	_turnMinusIndex(turnMinus) {
	FactoryIO::internal::checkModbusAddr(rollPlus, "modbusAdress rollPlus is required to use turntable");
	FactoryIO::internal::checkModbusAddr(rollMinus, "modbusAdress rollMinus is required to use turntable");
	FactoryIO::internal::checkModbusAddr(frontLimit, "modbusAdress frontLimit is required to use turntable");
	FactoryIO::internal::checkModbusAddr(backLimit, "modbusAdress backLimit is required to use turntable");
	FactoryIO::internal::checkModbusAddr(limit0, "modbusAdress limit0 is required to use turntable");
	FactoryIO::internal::checkModbusAddr(limit90, "modbusAdress limit90 is required to use turntable");

	if (mode == turntableMode_t::MONOSTABLE) {
		if (turn == FactoryIO::NO_MODBUS_ADDR)
			throw std::domain_error("Unable to activate mode MONOSTABLE when no turn address is provided");
	}

	if (mode == turntableMode_t::BISTABLE) {
		if (turnPlus == FactoryIO::NO_MODBUS_ADDR)
			throw std::domain_error("Unable to activate mode BISTABLE when no turnPlus address is provided");
		if (turnMinus == FactoryIO::NO_MODBUS_ADDR)
			throw std::domain_error("Unable to activate mode BISTABLE when no turnMinus address is provided");
	}
}

void FactoryIO::turntable_t::update() {
	switch (_state) {
	case FactoryIO::turntable_t::state_t::TURN_PICKUP:
		setConvayingDirection(_pickupPos);
		if (turnTurntable(shouldRotate(_pickupPos)))
			_state = state_t::WAIT_FOR_PICKUP;
		break;
	case FactoryIO::turntable_t::state_t::WAIT_FOR_PICKUP:
		if (pickupDone()) {
			_state = state_t::ROTATE;
			stopConveying();
		}
		break;
	case FactoryIO::turntable_t::state_t::ROTATE:
		if (turnTurntable(shouldRotate(_transferPos)))
			_state = state_t::TRANSFER;
		break;
	case FactoryIO::turntable_t::state_t::TRANSFER:
		setConvayingDirection(_transferPos);
		if (transferingComplete())
			_state = state_t::TURN_PICKUP;
		break;
	default:
		std::cout << "ERROR: turntable in invalid state. Reset to default." << std::endl;
		_state = state_t::TURN_PICKUP;
		break;
	}
}

bool FactoryIO::turntable_t::pickupDone() {
	bool sensorState = false;
	if (_pickupPos == direction_t::FRONT || _pickupPos == direction_t::RIGHT)
		_mb.modbus_read_input_bits(_backLimitIndex, 1, &sensorState);
	if (_pickupPos == direction_t::BACK || _pickupPos == direction_t::LEFT)
		_mb.modbus_read_input_bits(_frontLimitIndex, 1, &sensorState);
	return sensorState;
}

bool FactoryIO::turntable_t::transferingComplete() {
	bool sensorState = false;
	if (_transferPos == direction_t::FRONT || _transferPos == direction_t::RIGHT)
		_mb.modbus_read_input_bits(_frontLimitIndex, 1, &sensorState);
	if (_transferPos == direction_t::BACK || _transferPos == direction_t::LEFT)
		_mb.modbus_read_input_bits(_backLimitIndex, 1, &sensorState);

	if (sensorState == false && _transferSensorState == true) {
		_transferSensorState = sensorState;
		return true;
	}
	_transferSensorState = sensorState;
	return false;
}

bool FactoryIO::turntable_t::turnTurntable(bool turn) {
	bool limitSwitch = false;
	if (turn) 
		_mb.modbus_read_input_bits(_limit90Index, 1, &limitSwitch);
	else
		_mb.modbus_read_input_bits(_limit0Index, 1, &limitSwitch);

	if (limitSwitch) {
		_turning = false;
		if (_mode == turntableMode_t::MONOSTABLE)
			_mb.modbus_write_coil(_turnIndex, false);
		if (_mode == turntableMode_t::BISTABLE) {
			_mb.modbus_write_coil(_turnPlusIndex, false);
			_mb.modbus_write_coil(_turnMinusIndex, false);
		}
		return true;
	}

	// turn 
	if (!_turning || turn != _turningLastState) {
		_turning = true;
		_turningLastState = turn;
		if (_mode == turntableMode_t::MONOSTABLE)
			_mb.modbus_write_coil(_turnIndex, turn);
		if (_mode == turntableMode_t::BISTABLE) {
			_mb.modbus_write_coil(_turnPlusIndex, turn);
			_mb.modbus_write_coil(_turnMinusIndex, !turn);
		}
	}
	return false;
}

bool FactoryIO::turntable_t::shouldRotate(direction_t direction) {
	return direction == direction_t::LEFT || direction == direction_t::RIGHT;
}

void FactoryIO::turntable_t::setConvayingDirection(direction_t direction) {
	if (_lastConvayingState != (direction == direction_t::FRONT || direction == direction_t::RIGHT) || !_convaying) {
		_convaying = true;
		_lastConvayingState = (direction == direction_t::FRONT || direction == direction_t::RIGHT);
		_mb.modbus_write_coil(_rollPlusIndex, (direction == direction_t::FRONT || direction == direction_t::RIGHT));
		_mb.modbus_write_coil(_rollMinusIndex, !(direction == direction_t::FRONT || direction == direction_t::RIGHT));
	}
}

void FactoryIO::turntable_t::stopConveying() {
	_convaying = false;
	_mb.modbus_write_coil(_rollPlusIndex, false);
	_mb.modbus_write_coil(_rollMinusIndex, false);
}
