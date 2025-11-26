#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FactoryIO::internal::testing;

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(stopBlade) {
		TEST_METHOD(usage) {
			constexpr FactoryIO::modbusAddr_t stopBladeAddr = 24;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::stopBlade_t stopBlade(mb, stopBladeAddr);
			stopBlade.setStopBladeState(true);
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(stopBladeAddr, mb.getModbus()));
			stopBlade.setStopBladeState(false);
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(stopBladeAddr, mb.getModbus()));
		}
		TEST_METHOD(exceptions) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::stopBlade_t stopBlade(mb, FactoryIO::NO_MODBUS_ADDR);
					stopBlade.setStopBladeState(true);
				}, L"No exception when setStopBladeState is called and no Modbus Address was provided for stopBlade"
			);
		}
	};
}