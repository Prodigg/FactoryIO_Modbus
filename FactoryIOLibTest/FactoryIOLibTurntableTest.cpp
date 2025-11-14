#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FactoryIO::internal::testing;

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(turntable) {
		TEST_METHOD(monostable) {
			constexpr FactoryIO::modbusAddr_t turntableRollPlus = 0;
			constexpr FactoryIO::modbusAddr_t turntableRollMinus = 0;
			constexpr FactoryIO::modbusAddr_t turntableFrontLimit = 0;
			constexpr FactoryIO::modbusAddr_t turntableBackLimit = 0;
			constexpr FactoryIO::modbusAddr_t turntableLimit0 = 0;
			constexpr FactoryIO::modbusAddr_t turntableLimit90 = 0;
			constexpr FactoryIO::modbusAddr_t turntableTurn = 0;
			constexpr FactoryIO::modbusAddr_t turntableTurnPlus = 0;
			constexpr FactoryIO::modbusAddr_t turntableTurnMinus = 0;

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::turntable_t turntable(
				mb,
				turntableRollPlus,
				turntableRollMinus,
				turntableFrontLimit,
				turntableBackLimit,
				turntableLimit0,
				turntableLimit90,
				FactoryIO::turntableMode_t::MONOSTABLE,
				turntableTurn,
				turntableTurnPlus,
				turntableTurnMinus
			);
			// TODO: develop test with emmiter and convayor to test turntable
			Assert::Fail(L"Develop tests here");
			mb.modbus_close();
		}
		TEST_METHOD(bistable) {
			constexpr FactoryIO::modbusAddr_t turntableRollPlus = 0;
			constexpr FactoryIO::modbusAddr_t turntableRollMinus = 0;
			constexpr FactoryIO::modbusAddr_t turntableFrontLimit = 0;
			constexpr FactoryIO::modbusAddr_t turntableBackLimit = 0;
			constexpr FactoryIO::modbusAddr_t turntableLimit0 = 0;
			constexpr FactoryIO::modbusAddr_t turntableLimit90 = 0;
			constexpr FactoryIO::modbusAddr_t turntableTurn = 0;
			constexpr FactoryIO::modbusAddr_t turntableTurnPlus = 0;
			constexpr FactoryIO::modbusAddr_t turntableTurnMinus = 0;

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::turntable_t turntable(
				mb,
				turntableRollPlus,
				turntableRollMinus,
				turntableFrontLimit,
				turntableBackLimit,
				turntableLimit0,
				turntableLimit90,
				FactoryIO::turntableMode_t::BISTABLE,
				turntableTurn,
				turntableTurnPlus,
				turntableTurnMinus
			);
			// TODO: develop test with emmiter and convayor to test turntable
			Assert::Fail(L"Develop tests here");
			mb.modbus_close();
		}
		TEST_METHOD(exceptions) {
			constexpr FactoryIO::modbusAddr_t turntableRollPlus = 0;
			constexpr FactoryIO::modbusAddr_t turntableRollMinus = 0;
			constexpr FactoryIO::modbusAddr_t turntableFrontLimit = 0;
			constexpr FactoryIO::modbusAddr_t turntableBackLimit = 0;
			constexpr FactoryIO::modbusAddr_t turntableLimit0 = 0;
			constexpr FactoryIO::modbusAddr_t turntableLimit90 = 0;
			constexpr FactoryIO::modbusAddr_t turntableTurn = 0;
			constexpr FactoryIO::modbusAddr_t turntableTurnPlus = 0;
			constexpr FactoryIO::modbusAddr_t turntableTurnMinus = 0;

			modbus mb = modbus("127.0.0.1", 502);
			Assert::ExpectException < std::runtime_error, std::function<void(void)>>(
				[&]() -> void {
					FactoryIO::turntable_t turntable(
						mb,
						FactoryIO::NO_MODBUS_ADDR,
						turntableRollMinus,
						turntableFrontLimit,
						turntableBackLimit,
						turntableLimit0,
						turntableLimit90,
						FactoryIO::turntableMode_t::BISTABLE,
						turntableTurn,
						turntableTurnPlus,
						turntableTurnMinus);
				}, L"no exception when RollPlus is provided no modbus address"
			);

			Assert::ExpectException < std::runtime_error, std::function<void(void)>>(
				[&]() -> void {
					FactoryIO::turntable_t turntable(
						mb,
						turntableRollPlus,
						FactoryIO::NO_MODBUS_ADDR,
						turntableFrontLimit,
						turntableBackLimit,
						turntableLimit0,
						turntableLimit90,
						FactoryIO::turntableMode_t::BISTABLE,
						turntableTurn,
						turntableTurnPlus,
						turntableTurnMinus);
				}, L"no exception when RollMinus is provided no modbus address"
			);

			Assert::ExpectException < std::runtime_error, std::function<void(void)>>(
				[&]() -> void {
					FactoryIO::turntable_t turntable(
						mb,
						turntableRollPlus,
						turntableRollMinus,
						FactoryIO::NO_MODBUS_ADDR,
						turntableBackLimit,
						turntableLimit0,
						turntableLimit90,
						FactoryIO::turntableMode_t::BISTABLE,
						turntableTurn,
						turntableTurnPlus,
						turntableTurnMinus);
				}, L"no exception when FrontLimit is provided no modbus address"
			);

			Assert::ExpectException < std::runtime_error, std::function<void(void)>>(
				[&]() -> void {
					FactoryIO::turntable_t turntable(
						mb,
						turntableRollPlus,
						turntableRollMinus,
						turntableFrontLimit,
						FactoryIO::NO_MODBUS_ADDR,
						turntableLimit0,
						turntableLimit90,
						FactoryIO::turntableMode_t::BISTABLE,
						turntableTurn,
						turntableTurnPlus,
						turntableTurnMinus);
				}, L"no exception when BackLimit is provided no modbus address"
			);

			Assert::ExpectException < std::runtime_error, std::function<void(void)>>(
				[&]() -> void {
					FactoryIO::turntable_t turntable(
						mb,
						turntableRollPlus,
						turntableRollMinus,
						turntableFrontLimit,
						turntableBackLimit,
						FactoryIO::NO_MODBUS_ADDR,
						turntableLimit90,
						FactoryIO::turntableMode_t::BISTABLE,
						turntableTurn,
						turntableTurnPlus,
						turntableTurnMinus);
				}, L"no exception when Limit0 is provided no modbus address"
			);

			Assert::ExpectException < std::runtime_error, std::function<void(void)>>(
				[&]() -> void {
					FactoryIO::turntable_t turntable(
						mb,
						turntableRollPlus,
						turntableRollMinus,
						turntableFrontLimit,
						turntableBackLimit,
						turntableLimit0,
						FactoryIO::NO_MODBUS_ADDR,
						FactoryIO::turntableMode_t::BISTABLE,
						turntableTurn,
						turntableTurnPlus,
						turntableTurnMinus);
				}, L"no exception when Limit90 is provided no modbus address"
			);


		}
	};
}