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
			std::chrono::steady_clock::time_point begin;
			bool ModbusServerSireneOn = false;

			///// check if sirene turnes on

			alarmSirene.setSireneState(true);

			begin = std::chrono::steady_clock::now();
			while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() >= 50) {
			}
			
			mb.modbus_read_coils(0, 1, &ModbusServerSireneOn);
			Assert::AreEqual(true, ModbusServerSireneOn, L"sirene didn't turn on");

			///// check if sirene turnes off

			alarmSirene.setSireneState(false);

			begin = std::chrono::steady_clock::now();
			while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() >= 50) {
			}

			mb.modbus_read_coils(0, 1, &ModbusServerSireneOn);
			Assert::AreEqual(false, ModbusServerSireneOn, L"sirene didn't turn off");
			mb.modbus_close();
		}
	};
}
