#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FactoryIOLibTest {
	TEST_CLASS(FactoryIOLibTest) {
		
	public:

		TEST_METHOD(alarmSirene) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::alarmSiren_t alarmSirene(mb, 0);
			bool ModbusServerSireneOn = false;

			///// check if sirene turnes on

			alarmSirene.setSireneState(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			mb.modbus_read_coils(0, 1, &ModbusServerSireneOn);
			Assert::AreEqual(true, ModbusServerSireneOn, L"sirene didn't turn on");

			///// check if sirene turnes off

			alarmSirene.setSireneState(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			mb.modbus_read_coils(0, 1, &ModbusServerSireneOn);
			Assert::AreEqual(false, ModbusServerSireneOn, L"sirene didn't turn off");
			mb.modbus_close();
		}

		TEST_METHOD(warningLight) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::WarningLight_t warningLight(mb, 1);
			bool ModbusServerLightOn = false;

			///// check if light turnes on

			warningLight.setLightState(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			mb.modbus_read_coils(1, 1, &ModbusServerLightOn);
			Assert::AreEqual(true, ModbusServerLightOn, L"sirene didn't turn on");

			///// check if light turnes off

			warningLight.setLightState(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			mb.modbus_read_coils(0, 1, &ModbusServerLightOn);
			Assert::AreEqual(false, ModbusServerLightOn, L"sirene didn't turn off");
			mb.modbus_close();
		}
	};
}
