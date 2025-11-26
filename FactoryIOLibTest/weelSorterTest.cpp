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
			constexpr FactoryIO::modbusAddr_t plusAddr = 14;
			constexpr FactoryIO::modbusAddr_t minusAddr = 15;
			constexpr FactoryIO::modbusAddr_t leftAddr = 16;
			constexpr FactoryIO::modbusAddr_t rightAddr = 17;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::weelSorter_t weelSorter(mb, plusAddr, minusAddr, leftAddr, rightAddr);

			weelSorter.moveForward();
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(plusAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(leftAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(rightAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(minusAddr, mb.getModbus()));

			weelSorter.moveLeft();
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(plusAddr, mb.getModbus()));
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(leftAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(rightAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(minusAddr, mb.getModbus()));

			weelSorter.moveRight();
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(plusAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(leftAddr, mb.getModbus()));
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(rightAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(minusAddr, mb.getModbus()));

			weelSorter.moveBackward();
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(plusAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(leftAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(rightAddr, mb.getModbus()));
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(minusAddr, mb.getModbus()));

			weelSorter.stop();
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(plusAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(leftAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(rightAddr, mb.getModbus()));
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(minusAddr, mb.getModbus()));

		}

		TEST_METHOD(exceptions) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::weelSorter_t weelSorter(mb, FactoryIO::NO_MODBUS_ADDR, 0, 0, 0);
				}, L"No exception when weelSorter is created and no Modbus Address was provided for plusAddr"
			);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::weelSorter_t weelSorter(mb, 0, 0, FactoryIO::NO_MODBUS_ADDR, 0);
					weelSorter.moveLeft();
				}, L"No exception when moveLeft is called and no Modbus Address was provided for leftAddr"
			);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::weelSorter_t weelSorter(mb, 0, 0, 0, FactoryIO::NO_MODBUS_ADDR);
					weelSorter.moveRight();
				}, L"No exception when moveRight is called and no Modbus Address was provided for rightAddr"
			);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::weelSorter_t weelSorter(mb, 0, FactoryIO::NO_MODBUS_ADDR, 0, 0);
					weelSorter.moveBackward();
				}, L"No exception when moveBackward is called and no Modbus Address was provided for minusAddr"
			);
		}
	};
}