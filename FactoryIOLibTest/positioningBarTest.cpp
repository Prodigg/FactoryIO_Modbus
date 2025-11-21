#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FactoryIO::internal::testing;

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(positioningBar) {
		TEST_METHOD(usage) {
			constexpr FactoryIO::modbusAddr_t clampAddr = 0;
			constexpr FactoryIO::modbusAddr_t raiseAddr = 0;
			constexpr FactoryIO::modbusAddr_t clampedLimitAddr = 0;
			constexpr FactoryIO::modbusAddr_t verticalLimitAddr = 0;
			Assert::Fail(L"add correct address");

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::positioningBar_t posBar(mb, clampAddr, raiseAddr, clampedLimitAddr, verticalLimitAddr);
			
			posBar.clamp();
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(clampAddr, mb));
			Assert::IsTrue(posBar.isClamped());
			posBar.release();
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(clampAddr, mb));
			Assert::IsFalse(posBar.isClamped());

			posBar.raise();
			Assert::IsTrue(FactoryIO::internal::testing::getModbusCoilState(raiseAddr, mb));
			Assert::IsTrue(posBar.isRaised());
			posBar.lower();
			Assert::IsFalse(FactoryIO::internal::testing::getModbusCoilState(raiseAddr, mb));
			Assert::IsFalse(posBar.isRaised());

			mb.modbus_close();
		}
		TEST_METHOD(exceptions) {
			modbus mb = modbus("127.0.0.1", 502);
			FactoryIO::positioningBar_t posBar(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					posBar.clamp();
				}, L"No exception when clamp is called and no Modbus Address was provided"
			);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					posBar.release();
				}, L"No exception when release is called and no Modbus Address was provided"
			);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					posBar.lower();
				}, L"No exception when lower is called and no Modbus Address was provided"
			);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					posBar.raise();
				}, L"No exception when raise is called and no Modbus Address was provided"
			);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					posBar.isClamped();
				}, L"No exception when isClamped is called and no Modbus Address was provided"
			);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					posBar.isRaised();
				}, L"No exception when isRaised is called and no Modbus Address was provided"
			);

			mb.modbus_close();
		}
	};
}