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
		TEST_METHOD(push) {
			Assert::Fail(L"write test cases for pusher");
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