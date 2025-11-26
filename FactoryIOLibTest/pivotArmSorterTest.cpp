#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FactoryIO::internal::testing;

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(pivotArm) {
		TEST_METHOD(arm) {
			constexpr FactoryIO::modbusAddr_t pivotArmAddr = 18;
			
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::pivotArmSorter_t pivotArmSorter(mb, 0, 0, pivotArmAddr);
			
			pivotArmSorter.setArmPosition(true);
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(pivotArmAddr, mb.getModbus()));

			pivotArmSorter.setArmPosition(false);
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(pivotArmAddr, mb.getModbus()));
		}
		TEST_METHOD(exceptions) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::pivotArmSorter_t pivotArm(mb, 0, 0, FactoryIO::NO_MODBUS_ADDR);
					pivotArm.setArmPosition(true);
				}, L"No exception when isGateOpen is called and no Modbus Address was provided for opend"
			);
		}
	};
}