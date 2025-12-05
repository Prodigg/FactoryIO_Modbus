#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"
namespace FactoryIO {
	enum class turntableMode_t {
		MONOSTABLE = 0,
		BISTABLE
	};

	class turntable_t {
	public:
		turntable_t(ModbusProvider_t& mb,
			modbusAddr_t rollPlus,
			modbusAddr_t rollMinus,
			modbusAddr_t frontLimit,
			modbusAddr_t backLimit,
			modbusAddr_t limit0,
			modbusAddr_t limit90,
			turntableMode_t mode,
			modbusAddr_t turn,
			modbusAddr_t turnPlus,
			modbusAddr_t turnMinus);

		turntable_t& pickupLeft() { _pickupPos = direction_t::LEFT; return *this; };
		turntable_t& pickupRight() { _pickupPos = direction_t::RIGHT; return *this; };
		turntable_t& pickupFront() { _pickupPos = direction_t::FRONT; return *this; };
		turntable_t& pickupBack() { _pickupPos = direction_t::BACK; return *this; };

		void transferLeft() { _transferPos = direction_t::LEFT; };
		void transferRight() { _transferPos = direction_t::RIGHT; };
		void transferFront() { _transferPos = direction_t::FRONT; };
		void transferBack() { _transferPos = direction_t::BACK; };

		bool isTurntableReadyForPickup() const { return _state == state_t::WAIT_FOR_PICKUP; }

		void update();
	private:
		enum class direction_t {
			LEFT = 0,
			RIGHT, 
			FRONT, 
			BACK
		};

		enum class state_t {
			TURN_PICKUP = 0,
			WAIT_FOR_PICKUP,
			ROTATE,
			TRANSFER
		};

		ModbusProvider_t& _mb;
		modbusAddr_t _rollPlusIndex;
		modbusAddr_t _rollMinusIndex;
		modbusAddr_t _frontLimitIndex;
		modbusAddr_t _backLimitIndex;
		modbusAddr_t _limit0Index;
		modbusAddr_t _limit90Index;
		modbusAddr_t _turnIndex;
		modbusAddr_t _turnPlusIndex;
		modbusAddr_t _turnMinusIndex;

		turntableMode_t _mode;

		direction_t _pickupPos = direction_t::FRONT;
		direction_t _transferPos = direction_t::BACK;

		state_t _state = state_t::TURN_PICKUP;

		bool _transferSensorState = false;
		bool _turning = false;
		bool _turningLastState = false;

		bool _convaying = false;
		bool _lastConvayingState = false;

		bool pickupDone();
		bool transferingComplete();
		bool turnTurntable(bool turn);
		inline bool shouldRotate(direction_t direction);
		void setConvayingDirection(direction_t direction);
		void stopConveying();
	};
} 