#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

bool getModbusCoilState(FactoryIO::modbusAddr_t addr, modbus& mb) {
	bool tmp = false;
	mb.modbus_read_coils(addr, 1, &tmp);
	return tmp;
}

namespace FactoryIOLibTest {
	TEST_CLASS(alarmSirene) {

	public:

		TEST_METHOD(turnOnOff) {
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
	};

	TEST_CLASS(warningLight) {
		TEST_METHOD(turnOnOff) {
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

	TEST_CLASS(stackLight) {
		TEST_METHOD(greenLight) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::StackLight_t stackLight(mb, 2, 3, 4);

			///// check if green light turnes on

			stackLight.setGreenLight(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		
			Assert::AreEqual(true, getModbusCoilState(2, mb), L"green light didn't turn on");
			Assert::AreEqual(false, getModbusCoilState(3, mb), L"orange light turned on");
			Assert::AreEqual(false, getModbusCoilState(4, mb), L"red light turned on");

			///// check if light turnes off

			stackLight.setGreenLight(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb), L"green light didn't turn off");
			Assert::AreEqual(false, getModbusCoilState(3, mb), L"orange light turned on");
			Assert::AreEqual(false, getModbusCoilState(4, mb), L"red light turned on");

			mb.modbus_close();

		}

		TEST_METHOD(orangeLight) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::StackLight_t stackLight(mb, 2, 3, 4);

			///// check if green light turnes on

			stackLight.setOrangeLight(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb), L"green light turned on");
			Assert::AreEqual(true, getModbusCoilState(3, mb), L"orange light didn't turn on");
			Assert::AreEqual(false, getModbusCoilState(4, mb), L"red light turned on");

			///// check if light turnes off

			stackLight.setOrangeLight(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb), L"green light turned on");
			Assert::AreEqual(false, getModbusCoilState(3, mb), L"orange light didn't turn off");
			Assert::AreEqual(false, getModbusCoilState(4, mb), L"red light turned on");

			mb.modbus_close();

		}

		TEST_METHOD(RedLight) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}
			FactoryIO::StackLight_t stackLight(mb, 2, 3, 4);

			///// check if green light turnes on

			stackLight.setRedLight(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb), L"green light turned on");
			Assert::AreEqual(false, getModbusCoilState(3, mb), L"orange light turned on");
			Assert::AreEqual(true, getModbusCoilState(4, mb), L"red light didn't turn on");

			///// check if light turnes off

			stackLight.setRedLight(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb), L"green light turned on");
			Assert::AreEqual(false, getModbusCoilState(3, mb), L"orange light turned on");
			Assert::AreEqual(false, getModbusCoilState(4, mb), L"red light didn't turn off");

			mb.modbus_close();

		}
	};
}
