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
			constexpr uint16_t scaleFactor = 100;
			constexpr FactoryIO::modbusAddr_t weigherSignalAddr = 2;
			
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::convayorScale_t scale(mb, 0, 0, weigherSignalAddr, scaleFactor);

			Assert::AreEqual(0.1, scale.getCurrentWeight(), 0.05);
		}
		TEST_METHOD(exceptions) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);
			FactoryIO::convayorScale_t scale(mb, 0, 0, FactoryIO::NO_MODBUS_ADDR, 0);
			Assert::ExpectException < std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					scale.getCurrentWeight();
				}, L"no exception when callig get weight when no addres is providet."
			);
		}

	};
}