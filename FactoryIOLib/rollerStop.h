#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"
namespace FactoryIO {
	class rollerStop_t {
	public:
		/*!
		 * @brief initialize roller stop
		 * @param mb reference to modbus provider
		 * @param rollerStopIndex 
		 */
		rollerStop_t(ModbusProvider_t& mb, modbusAddr_t rollerStopIndex) : _rollerStopActiveAddr(rollerStopIndex), _mb(mb) {}

		/*!
		 * @brief sets the roller state
		 * @param state state of roller
		 * @exception std::runtime_error if rollerStopIndex is NO_MODBUS_ADDR
		 */
		void setRollerState(bool state);
	private:
		ModbusProvider_t& _mb;
		modbusAddr_t _rollerStopActiveAddr;
	};
}