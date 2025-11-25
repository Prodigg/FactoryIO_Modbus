#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FactoryIO::internal::testing;

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(convayorScale) {

		TEST_METHOD(scale) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			Assert::Fail(L"add test");

			mb.modbus_close();
		}
		TEST_METHOD(exceptions) {
			modbus mb = modbus("127.0.0.1", 502);
			FactoryIO::convayorScale_t scale(mb, 0, 0, FactoryIO::NO_MODBUS_ADDR, 0);
			Assert::ExpectException < std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					scale.getCurrentWeight();
				}, L"no exception when callig get weight when no addres is providet."
			);
		}

	};
}