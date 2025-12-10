#pragma once
#include "FactoryIOGeneral.h"
#include "modbusProvider.h"

namespace FactoryIO {
	class chainTransfer_t {
	public:
		/*!
		 * @brief initialize chainTransfer object
		 * @param mb reference to Modbus providor
		 * @param chaintransferPlus if not present use FactoryIO::NO_MODBUS_ADDR
		 * @param chaintransferMinus if not present use FactoryIO::NO_MODBUS_ADDR
		 * @param chaintransferLeft if not present use FactoryIO::NO_MODBUS_ADDR
		 * @param chaintransferRight if not present use FactoryIO::NO_MODBUS_ADDR
		 */
		chainTransfer_t(ModbusProvider_t& mb, modbusAddr_t chaintransferPlus, modbusAddr_t chaintransferMinus, modbusAddr_t chaintransferLeft, modbusAddr_t chaintransferRight);
		
		/*!
		 * @brief move the object on the chaintransfer forward
		 * @exception std::runtime_error if chaintransferPlus is NO_MODBUS_ADDR
		 */
		void moveForward();
		
		/*!
		 * @brief move the object on the chaintransfer backwards
		 * @exception std::runtime_error if chaintransferMinus is NO_MODBUS_ADDR
		 */
		void moveBackward();

		/*!
		 * @brief move the object on the chaintransfer left
		 * @exception std::runtime_error if chaintransferLeft is NO_MODBUS_ADDR
		 */
		void moveLeft();

		/*!
		 * @brief move the object on the chaintransfer right
		 * @exception std::runtime_error if chaintransferRight is NO_MODBUS_ADDR
		 */
		void moveRight();

		/*!
		 * @brief stops all movment
		 */
		void stop();
	private:
		ModbusProvider_t& _mb;
		modbusAddr_t _chaintransferPlusAddr;
		modbusAddr_t _chaintransferMinusAddr;
		modbusAddr_t _chaintransferLeftAddr;
		modbusAddr_t _chaintransferRightAddr;
	};
}