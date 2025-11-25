#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FactoryIO::internal::testing;

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(pusher) {
		TEST_METHOD(monostable) {
			constexpr FactoryIO::modbusAddr_t monostablePushAddr = 21;
			constexpr FactoryIO::modbusAddr_t monostableFrontLimitAddr = 5;
			constexpr FactoryIO::modbusAddr_t monostableBackLimitAddr = 4;

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::pusher_t pusher = FactoryIO::pusher_t::constructMonostable(mb, monostablePushAddr, monostableFrontLimitAddr, monostableBackLimitAddr);
			
			pusher.push(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Assert::IsTrue(pusher.isAtBack());

			pusher.push(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Assert::IsTrue(pusher.isAtFront());

			pusher.push(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Assert::IsTrue(pusher.isAtBack());

			mb.modbus_close();
		}
		TEST_METHOD(bistable) {
			constexpr FactoryIO::modbusAddr_t bistablePushPlusAddr = 22;
			constexpr FactoryIO::modbusAddr_t bistablePushMinusAddr = 23;
			constexpr FactoryIO::modbusAddr_t bistableFrontLimitAddr = 7;
			constexpr FactoryIO::modbusAddr_t bistableBackLimitAddr = 6;

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::pusher_t pusher = FactoryIO::pusher_t::constructBistable(mb, bistablePushPlusAddr, bistablePushMinusAddr, bistableFrontLimitAddr, bistableBackLimitAddr);

			pusher.push(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			Assert::IsTrue(pusher.isAtBack());

			pusher.push(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			Assert::IsTrue(pusher.isAtFront());

			pusher.push(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			Assert::IsTrue(pusher.isAtBack());

			mb.modbus_close();
		}
		TEST_METHOD(analog) {
			constexpr FactoryIO::modbusAddr_t analogPusherSetpointAddr = 5;
			constexpr FactoryIO::modbusAddr_t analogPusherPositionAddr = 3;
			constexpr uint16_t scaleFactor = 100;

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::pusher_t pusher = FactoryIO::pusher_t::constructAnalog(mb, analogPusherSetpointAddr, analogPusherPositionAddr, scaleFactor);

			pusher.push(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Assert::IsTrue(pusher.isAtBack());

			pusher.push(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Assert::IsTrue(pusher.isAtFront());

			pusher.push(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Assert::IsTrue(pusher.isAtBack());

			mb.modbus_close();
		}
		TEST_METHOD(analogDigital) {
			constexpr FactoryIO::modbusAddr_t analogDigitalPusherSetpointAddr = 6;
			constexpr FactoryIO::modbusAddr_t analogDigitalPusherPositionAddr = 4;
			constexpr FactoryIO::modbusAddr_t analogDigitalFrontLimitAddr = 9;
			constexpr FactoryIO::modbusAddr_t analogDigitalBackLimitAddr = 8;
			constexpr uint16_t scaleFactor = 100;

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::pusher_t pusher = FactoryIO::pusher_t::constructAnalogDigital(
				mb, 
				analogDigitalPusherSetpointAddr, 
				analogDigitalPusherPositionAddr, 
				analogDigitalFrontLimitAddr, 
				analogDigitalBackLimitAddr,
				scaleFactor
			);

			pusher.push(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Assert::IsTrue(pusher.isAtBack());

			pusher.push(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Assert::IsTrue(pusher.isAtFront());

			pusher.push(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Assert::IsTrue(pusher.isAtBack());

			mb.modbus_close();
		}
		TEST_METHOD(exceptions) {
			modbus mb = modbus("127.0.0.1", 502);
			using FactoryIO::NO_MODBUS_ADDR;
			FactoryIO::pusher_t pusherMonostable = FactoryIO::pusher_t::constructMonostable(mb, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR);
			FactoryIO::pusher_t pusherBistable = FactoryIO::pusher_t::constructBistable(mb, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR);
			FactoryIO::pusher_t pusherAnalog = FactoryIO::pusher_t::constructAnalog(mb, NO_MODBUS_ADDR, NO_MODBUS_ADDR, 1);
			FactoryIO::pusher_t pusherAnalogDigital = FactoryIO::pusher_t::constructAnalogDigital(mb, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR, NO_MODBUS_ADDR, 1);

			Assert::ExpectException<std::domain_error, std::function<void(void)>>(
				[&](void) -> void {
					pusherMonostable.getPosition();
				}, L"No exception when getPosition is called with monostable"
			);
			Assert::ExpectException<std::domain_error, std::function<void(void)>>(
				[&](void) -> void {
					pusherBistable.getPosition();
				}, L"No exception when getPosition is called with bistable"
			);
			
			Assert::ExpectException<std::domain_error, std::function<void(void)>>(
				[&](void) -> void {
					pusherMonostable.targetPosition(1);
				}, L"No exception when targetPosition is called with monostable"
			);
			Assert::ExpectException<std::domain_error, std::function<void(void)>>(
				[&](void) -> void {
					pusherBistable.targetPosition(1);
				}, L"No exception when targetPosition is called with bistable"
			);
			mb.modbus_close();
		}
	};
}