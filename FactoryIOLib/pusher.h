#pragma once
#include "FactoryIOGeneral.h"
#include <functional>
#include <optional>

namespace FactoryIO {
	enum class pusherMode_t {
		MONOSTABLE = 0, 
		BISTABLE, 
		ANALOG,
		DIGITAL_ANALOG
	};

	enum class pusherSpeedFunction_t {
		LINIAR = 0,
		EXPONENTIAL
	};

	class pusher_t {
	public:
		pusher_t(
			modbus& mb, 
			pusherMode_t mode, 
			modbusAddr_t pusherAddr, 
			modbusAddr_t frontLimitAddr, 
			modbusAddr_t backLimitAddr, 
			modbusAddr_t pusherPlusAddr, 
			modbusAddr_t pusherMinusAddr, 
			modbusAddr_t pusherSetpointAddr,
			modbusAddr_t pusherPositionAddr, 
			uint16_t scaleFactor);

		static pusher_t constructMonostable(modbus& mb, modbusAddr_t pusherAddr, modbusAddr_t frontLimitAddr, modbusAddr_t backLimitAddr);
		static pusher_t constructBistable(modbus& mb, modbusAddr_t pusherPlusAddr, modbusAddr_t pusherMinusAddr, modbusAddr_t frontLimitAddr, modbusAddr_t backLimitAddr);
		static pusher_t constructAnalog(modbus& mb, modbusAddr_t pusherSetpointAddr, modbusAddr_t pusherPositionAddr, uint16_t scalefactor);
		static pusher_t constructAnalogDigital(modbus& mb, modbusAddr_t pusherSetpointAddr, modbusAddr_t pusherPositionAddr, modbusAddr_t frontLimitAddr, modbusAddr_t backLimitAddr, uint16_t scalefactor);

		void push(bool forward);
		bool isAtFront();
		bool isAtBack();

		float getPosition();
		void setPushSpeed(float speed);
		bool targetPosition(float targetPos);
		void setPositioningParameters(float acceptableOffDistance) {_acceptableOffDistance = acceptableOffDistance; }
		void setSpeedFunction(std::function<float(float)> speedFunction);
		void setSpeedFunction(pusherSpeedFunction_t speedFunction);
		void update();
	private:
		uint16_t _pushSpeed = 5;
		float _targetPos = 0;

		std::optional<std::function<float(float)>> _speedFunction;
		float _acceptableOffDistance = 0.2;

		bool _moveToTargetPos = false;

		pusherMode_t _mode;

		modbusAddr_t _pusherAddr;
		modbusAddr_t _frontLimitAddr;
		modbusAddr_t _backLimitAddr;
		modbusAddr_t _pusherPlusAddr;
		modbusAddr_t _pusherMinusAddr;
		modbusAddr_t _pusherSetpointAddr;
		modbusAddr_t _pusherPositionAddr;

		uint16_t _scaleFactor;

		modbus& _mb;
	};
}