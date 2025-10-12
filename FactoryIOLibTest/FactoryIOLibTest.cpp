#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

bool getModbusCoilState(FactoryIO::modbusAddr_t addr, modbus& mb) {
	bool tmp = false;
	mb.modbus_read_coils(addr, 1, &tmp);
	return tmp;
}

uint16_t getModbusRegState(FactoryIO::modbusAddr_t addr, modbus& mb) {
	uint16_t tmp = 0;
	mb.modbus_read_holding_registers(addr, 1, &tmp);
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

		TEST_METHOD(noAddress) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::alarmSiren_t alarmSirene(mb, FactoryIO::NO_MODBUS_ADDR);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				alarmSirene.setSireneState(true);
				}, L"setSireneState did not rase a exception when no valid modbus addr is provided.");

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

		TEST_METHOD(noAddress) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::WarningLight_t warningLight(mb, FactoryIO::NO_MODBUS_ADDR);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				warningLight.setLightState(true);
				}, L"setSireneState did not rase a exception when no valid modbus addr is provided.");

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

		TEST_METHOD(noAddress) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::StackLight_t stackLight(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				stackLight.setGreenLight(true);
				}, L"setSireneState did not rase a exception when no valid modbus addr is provided.");

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				stackLight.setOrangeLight(true);
				}, L"setSireneState did not rase a exception when no valid modbus addr is provided.");

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				stackLight.setRedLight(true);
				}, L"setSireneState did not rase a exception when no valid modbus addr is provided.");

			mb.modbus_close();
		}
	};

	TEST_CLASS(emmiter) {
		TEST_METHOD(emmitState) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::emmiter_t emmiter(mb, 5, 1, 0);

			emmiter.emmit(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual(true, getModbusCoilState(5, mb), L"emmit didn't turn on");

			emmiter.emmit(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual(false, getModbusCoilState(5, mb), L"emmit didn't turn off");

			mb.modbus_close();
		}

		TEST_METHOD(emmitBase) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::emmiter_t emmiter(mb, 5, 1, 0);

			emmiter.emmit(true);
			emmiter.setBase({ FactoryIO::Bases_t::PALLET });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual(true, getModbusCoilState(5, mb), L"emmit didn't turn on");
			Assert::AreEqual((uint16_t)0b00000000000000001, getModbusRegState(0, mb), L"Pallet bit didn't turn on");

			emmiter.setBase({ FactoryIO::Bases_t::PALLET, FactoryIO::Bases_t::SQARE_PALLET });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000011, getModbusRegState(0, mb), L"emmiter base missmatch");

			emmiter.setBase({ FactoryIO::Bases_t::PALLET, FactoryIO::Bases_t::SQARE_PALLET, FactoryIO::Bases_t::STACKABLE_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000111, getModbusRegState(0, mb), L"emmiter base missmatch");

			emmiter.setBase({ FactoryIO::Bases_t::NO_BASE });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000000, getModbusRegState(0, mb), L"emmiter base didn't clear");

			emmiter.emmit(false);
			mb.modbus_close();
		}

		TEST_METHOD(emmitPart) {
			using namespace FactoryIO;
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::emmiter_t emmiter(mb, 5, 1, 0);

			emmiter.emmit(true);
			emmiter.setParts({ Parts_t::SMALL_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual(true, getModbusCoilState(5, mb), L"emmit didn't turn on");
			Assert::AreEqual((uint16_t)0b00000000000000001, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX, Parts_t::MEDIUM_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000011, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX, Parts_t::MEDIUM_BOX, Parts_t::LARGE_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX, Parts_t::MEDIUM_BOX, Parts_t::LARGE_BOX, Parts_t::PALLETIZING_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000001111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX, 
				Parts_t::MEDIUM_BOX, 
				Parts_t::LARGE_BOX, 
				Parts_t::PALLETIZING_BOX, 
				Parts_t::BLUE_RAW_MATERIAL 
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000011111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000111111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL,
				Parts_t::METTAL_RAW_MATERIAL
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000001111111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL,
				Parts_t::METTAL_RAW_MATERIAL,
				Parts_t::BLUE_PRODUCT_BASE
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000011111111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL,
				Parts_t::METTAL_RAW_MATERIAL,
				Parts_t::BLUE_PRODUCT_BASE,
				Parts_t::GREEN_PRODUCT_BASE
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000111111111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL,
				Parts_t::METTAL_RAW_MATERIAL,
				Parts_t::BLUE_PRODUCT_BASE,
				Parts_t::GREEN_PRODUCT_BASE,
				Parts_t::METTAL_PRODUCT_BASE
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000001111111111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL,
				Parts_t::METTAL_RAW_MATERIAL,
				Parts_t::BLUE_PRODUCT_BASE,
				Parts_t::GREEN_PRODUCT_BASE,
				Parts_t::METTAL_PRODUCT_BASE,
				Parts_t::BLUE_PRODUCT_LID
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000011111111111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL,
				Parts_t::METTAL_RAW_MATERIAL,
				Parts_t::BLUE_PRODUCT_BASE,
				Parts_t::GREEN_PRODUCT_BASE,
				Parts_t::METTAL_PRODUCT_BASE,
				Parts_t::BLUE_PRODUCT_LID,
				Parts_t::GREEN_PRODUCT_LID
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000111111111111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL,
				Parts_t::METTAL_RAW_MATERIAL,
				Parts_t::BLUE_PRODUCT_BASE,
				Parts_t::GREEN_PRODUCT_BASE,
				Parts_t::METTAL_PRODUCT_BASE,
				Parts_t::BLUE_PRODUCT_LID,
				Parts_t::GREEN_PRODUCT_LID,
				Parts_t::METTAL_PRODUCT_LID
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00001111111111111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL,
				Parts_t::METTAL_RAW_MATERIAL,
				Parts_t::BLUE_PRODUCT_BASE,
				Parts_t::GREEN_PRODUCT_BASE,
				Parts_t::METTAL_PRODUCT_BASE,
				Parts_t::BLUE_PRODUCT_LID,
				Parts_t::GREEN_PRODUCT_LID,
				Parts_t::METTAL_PRODUCT_LID,
				Parts_t::STACKABLE_BOX
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00011111111111111, getModbusRegState(1, mb), L"emmiter part missmatch");

			emmiter.emmit(false);
			mb.modbus_close();
		}

		TEST_METHOD(noAddress) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::emmiter_t emmiter(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				emmiter.emmit(true);
				}, L"emmit did not rase a exception when no valid modbus addr is provided.");

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				emmiter.setBase({});
				}, L"setBase did not rase a exception when no valid modbus addr is provided.");

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				emmiter.setParts({});
				}, L"setParts did not rase a exception when no valid modbus addr is provided.");

			mb.modbus_close();
		}
	};

	TEST_CLASS(BitfieldEnumMapper) {
		TEST_METHOD(Parts) {
			uint16_t returnBitfield = 0;
			for (uint16_t toTestBitfield = 0; toTestBitfield < 0b0011111111111111; toTestBitfield++) {
				returnBitfield = FactoryIO::internal::BitfieldEnumMapper_t::toBitfield(FactoryIO::internal::BitfieldEnumMapper_t::toParts(toTestBitfield));
				Assert::AreEqual((uint16_t)toTestBitfield, (uint16_t)returnBitfield, L"bitfield don't match, conversion not working.");
			}
		}

		TEST_METHOD(Bases) {
			uint16_t returnBitfield = 0;
			for (uint16_t toTestBitfield = 0; toTestBitfield < 0b0000000000000111; toTestBitfield++) {
				returnBitfield = FactoryIO::internal::BitfieldEnumMapper_t::toBitfield(FactoryIO::internal::BitfieldEnumMapper_t::toBases(toTestBitfield));
				Assert::AreEqual((uint16_t)toTestBitfield, (uint16_t)returnBitfield, L"bitfield don't match, conversion not working.");
			}
		}
	};

	TEST_CLASS(remover) {
		TEST_METHOD(toRemoveBase) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::remover_t remover(mb, 3, 2, 1, 0);

			uint16_t returnBitfield = 0;
			uint16_t modbusReadValue = 0;

			for (uint16_t toTestBitfield = 0; toTestBitfield < 0b0000000000000111; toTestBitfield++) {
				remover.setBasesToRemove(FactoryIO::internal::BitfieldEnumMapper_t::toBases(toTestBitfield));

				mb.modbus_read_holding_registers(2, 1, &modbusReadValue);
				Assert::AreEqual((uint16_t)toTestBitfield, (uint16_t)modbusReadValue, L"bitfield doesn't match with expected bitfield.");
			}

			remover.setBasesToRemove({});
			mb.modbus_close();
		}

		TEST_METHOD(toRemovePart) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::remover_t remover(mb, 3, 2, 1, 0);

			uint16_t returnBitfield = 0;
			uint16_t modbusReadValue = 0;

			for (uint16_t toTestBitfield = 0; toTestBitfield < 0b0011111111111111; toTestBitfield++) {
				remover.setPartsToRemove(FactoryIO::internal::BitfieldEnumMapper_t::toParts(toTestBitfield));

				mb.modbus_read_holding_registers(3, 1, &modbusReadValue);
				Assert::AreEqual((uint16_t)toTestBitfield, (uint16_t)modbusReadValue, L"bitfield doesn't match with expected bitfield.");
			}
			remover.setPartsToRemove({});
			mb.modbus_close();
		}

		TEST_METHOD(noAdress) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::remover_t remover(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				remover.setBasesToRemove({});
				}, L"setBasesToRemove did not rase a exception when no valid modbus addr is provided.");
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				remover.setPartsToRemove({});
				}, L"setPartsToRemove did not rase a exception when no valid modbus addr is provided.");
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				remover.getDetectedBases();
				}, L"getDetectedBases did not rase a exception when no valid modbus addr is provided.");
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				remover.getDetectedBases();
				}, L"getDetectedBases did not rase a exception when no valid modbus addr is provided.");

			mb.modbus_close();
		}
	};
}
