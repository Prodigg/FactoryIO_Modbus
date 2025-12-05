#include "turntable.h"
#include <iostream>
#include <stdexcept>

FactoryIO::turntable_t::turntable_t(
	ModbusProvider_t& mb,
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
		if (transferingComplete()) {
			_state = state_t::TURN_PICKUP;
			_transferSensorState = false;
		}
		break;
	default:
		std::cout << "ERROR: turntable in invalid state. Reset to default." << std::endl;
		_state = state_t::TURN_PICKUP;
		break;
	}
}

bool FactoryIO::turntable_t::pickupDone() {
	if (_pickupPos == direction_t::FRONT || _pickupPos == direction_t::RIGHT)
		return _mb.readInputBit(_backLimitIndex);
	if (_pickupPos == direction_t::BACK || _pickupPos == direction_t::LEFT)
		return _mb.readInputBit(_frontLimitIndex);
	return false;
}

bool FactoryIO::turntable_t::transferingComplete() {
	bool sensorState = false;
	if (_transferPos == direction_t::FRONT || _transferPos == direction_t::RIGHT)
		sensorState = _mb.readInputBit(_backLimitIndex);
	if (_transferPos == direction_t::BACK || _transferPos == direction_t::LEFT)
		sensorState = _mb.readInputBit(_frontLimitIndex);

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
		limitSwitch = _mb.readInputBit(_limit90Index);
	else
		limitSwitch = _mb.readInputBit(_limit0Index);

	if (limitSwitch) {
		_turning = false;
		if (_mode == turntableMode_t::MONOSTABLE)
			_mb.writeCoil(_turnIndex, false);
		if (_mode == turntableMode_t::BISTABLE) {
			_mb.writeCoil(_turnPlusIndex, false);
			_mb.writeCoil(_turnMinusIndex, false);
		}
		return true;
	}

	// turn 
	if (!_turning || turn != _turningLastState) {
		_turning = true;
		_turningLastState = turn;
		if (_mode == turntableMode_t::MONOSTABLE)
			_mb.writeCoil(_turnIndex, turn);
		if (_mode == turntableMode_t::BISTABLE) {
			_mb.writeCoil(_turnPlusIndex, turn);
			_mb.writeCoil(_turnMinusIndex, !turn);
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
		_lastConvayingState = !(direction == direction_t::FRONT || direction == direction_t::RIGHT);
		_mb.writeCoil(_rollPlusIndex, !(direction == direction_t::FRONT || direction == direction_t::RIGHT));
		_mb.writeCoil(_rollMinusIndex, (direction == direction_t::FRONT || direction == direction_t::RIGHT));
	}
}

void FactoryIO::turntable_t::stopConveying() {
	_convaying = false;
	_mb.writeCoil(_rollPlusIndex, false);
	_mb.writeCoil(_rollMinusIndex, false);
}
