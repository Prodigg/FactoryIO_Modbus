#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FactoryIO::internal::testing;

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(weelSorter) {
		TEST_METHOD(usage) {
			constexpr FactoryIO::modbusAddr_t plusAddr = 0;
			constexpr FactoryIO::modbusAddr_t leftAddr = 0;
			constexpr FactoryIO::modbusAddr_t rightAddr = 0;
			Assert::Fail(L"add correct modbus addresses");
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::weelSorter_t weelSorter(mb, plusAddr, leftAddr, rightAddr);

			weelSorter.moveForward();
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(plusAddr, mb));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(leftAddr, mb));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(rightAddr, mb));

			weelSorter.moveLeft();
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(plusAddr, mb));
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(leftAddr, mb));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(rightAddr, mb));

			weelSorter.moveRight();
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(plusAddr, mb));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(leftAddr, mb));
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(rightAddr, mb));

			weelSorter.stop();
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(plusAddr, mb));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(leftAddr, mb));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(rightAddr, mb));

			mb.modbus_close();
		}

		TEST_METHOD(exceptions) {
			modbus mb = modbus("127.0.0.1", 502);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::weelSorter_t weelSorter(mb, FactoryIO::NO_MODBUS_ADDR, 0, 0);
				}, L"No exception when weelSorter is created and no Modbus Address was provided for plusAddr"
			);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::weelSorter_t weelSorter(mb, 0, FactoryIO::NO_MODBUS_ADDR, 0);
				}, L"No exception when weelSorter is created and no Modbus Address was provided for leftAddr"
			);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::weelSorter_t weelSorter(mb, 0, 0, FactoryIO::NO_MODBUS_ADDR);
				}, L"No exception when weelSorter is created and no Modbus Address was provided for rightAddr"
			);

			mb.modbus_close();
		}
	};
}