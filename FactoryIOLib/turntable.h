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
		/*!
		 * @brief cunstruct turntable, consider using dedecated constructer and not full constructor
		 * @param mb 
		 * @param rollPlus 
		 * @param rollMinus 
		 * @param frontLimit 
		 * @param backLimit 
		 * @param limit0 
		 * @param limit90 
		 * @param mode 
		 * @param turn set FactoryIO::NO_MODBUS_ADDR when mode is FactoryIO::turntableMode_t::BISTABLE
		 * @param turnPlus set FactoryIO::NO_MODBUS_ADDR when mode is FactoryIO::turntableMode_t::MONOSTABLE
		 * @param turnMinus set FactoryIO::NO_MODBUS_ADDR when mode is FactoryIO::turntableMode_t::MONOSTABLE
		 * @exception std::domain_error if turn is NO_MODBUS_ADDR when mode is FactoryIO::turntableMode_t::MONOSTABLE
		 * @exception std::domain_error if turnPlus or turnMinus is NO_MODBUS_ADDR when mode is FactoryIO::turntableMode_t::BISTABLE
		 * @exception std::runtime_error if rollPlus, rollMinus, frontLimit, backLimit, limit0 or limit90 is NO_MODBUS_ADDR
		 */
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

		/*!
		 * @brief construct a monostable turntable
		 * @param mb 
		 * @param rollPlus 
		 * @param rollMinus 
		 * @param frontLimit 
		 * @param backLimit 
		 * @param limit0 
		 * @param limit90 
		 * @param turn 
		 * @return constructed object
		 * @exception std::runtime_error if any parameter is NO_MODBUS_ADDR
		 */
		static turntable_t constructMonostable(ModbusProvider_t& mb,
			modbusAddr_t rollPlus,
			modbusAddr_t rollMinus,
			modbusAddr_t frontLimit,
			modbusAddr_t backLimit,
			modbusAddr_t limit0,
			modbusAddr_t limit90,
			modbusAddr_t turn);

		/*!
		 * @brief construct a bistable turntable
		 * @param mb 
		 * @param rollPlus 
		 * @param rollMinus 
		 * @param frontLimit 
		 * @param backLimit 
		 * @param limit0 
		 * @param limit90 
		 * @param turnPlus 
		 * @param turnMinus 
		 * @return constructed object
		 * @exception std::runtime_error if any parameter is NO_MODBUS_ADDR
		 */
		static turntable_t constructBistable(ModbusProvider_t& mb,
			modbusAddr_t rollPlus,
			modbusAddr_t rollMinus,
			modbusAddr_t frontLimit,
			modbusAddr_t backLimit,
			modbusAddr_t limit0,
			modbusAddr_t limit90,
			modbusAddr_t turnPlus,
			modbusAddr_t turnMinus);

		/*!
		 * @brief set pickup location to Left
		 */
		turntable_t& pickupLeft() { _pickupPos = direction_t::LEFT; return *this; };

		/*!
		 * @brief set pickup location to Right
		 */
		turntable_t& pickupRight() { _pickupPos = direction_t::RIGHT; return *this; };

		/*!
		 * @brief set pickup location to Front
		 */
		turntable_t& pickupFront() { _pickupPos = direction_t::FRONT; return *this; };

		/*!
		 * @brief set pickup location to Back
		 */
		turntable_t& pickupBack() { _pickupPos = direction_t::BACK; return *this; };

		/*!
		 * @brief set transfer location to Left
		 */
		void transferLeft() { _transferPos = direction_t::LEFT; };

		/*!
		 * @brief set transfer Location to Right
		 */
		void transferRight() { _transferPos = direction_t::RIGHT; };

		/*!
		 * @brief set transfer Location to Front
		 */
		void transferFront() { _transferPos = direction_t::FRONT; };
		
		/*!
		 * @brief set transfer Location to Back
		 */
		void transferBack() { _transferPos = direction_t::BACK; };

		/*!
		 * @brief is the turntable waiting for new object at pickup location
		 * @return 
		 */
		bool isTurntableReadyForPickup() const { return _state == state_t::WAIT_FOR_PICKUP; }

		/*!
		 * @brief call this function as fast as possible when the turntable is used
		 */
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