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
			constexpr FactoryIO::modbusAddr_t convayorPlusAddr = 0;
			constexpr FactoryIO::modbusAddr_t convayorMinusAddr = 0;
			constexpr FactoryIO::modbusAddr_t pivotArmAddr = 0;
			
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::pivotArmSorter_t pivotArmSorter(mb, convayorPlusAddr, convayorMinusAddr, pivotArmAddr);
			Assert::Fail(L"write unit tests");

			mb.modbus_close();
		}
		TEST_METHOD(exceptions) {
			modbus mb = modbus("127.0.0.1", 502);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::pivotArmSorter_t pivotArm(mb, 0, 0, FactoryIO::NO_MODBUS_ADDR);
					pivotArm.setArmPosition(true);
				}, L"No exception when isGateOpen is called and no Modbus Address was provided for opend"
			);

			mb.modbus_close();
		}
	};
}