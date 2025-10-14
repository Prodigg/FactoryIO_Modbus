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

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(alarmSirene) {
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

	TEST_CLASS(Convayor) {
		TEST_METHOD(digital) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			bool readValue = 0;

			FactoryIO::Convayor_t convayor(mb, 6, 7, 8, 4, FactoryIO::ConvayorMode_t::DIGITAL);
			convayor.move(true);
			mb.modbus_read_coils(6, 1, &readValue);
			Assert::IsTrue(readValue, L"convayor didn't start moving.");

			convayor.move(false);
			mb.modbus_read_coils(6, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor didn't stop moving.");

			convayor.move(true);
			mb.modbus_read_coils(6, 1, &readValue);
			Assert::IsTrue(readValue, L"convayor didn't start moving.");

			convayor.stop();
			mb.modbus_read_coils(6, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor didn't stop moving.");

			mb.modbus_close();
		}

		TEST_METHOD(digitalPlusMinus) {
			constexpr FactoryIO::modbusAddr_t digitalAddr = 6;
			constexpr FactoryIO::modbusAddr_t digitalPlusAddr = 7;
			constexpr FactoryIO::modbusAddr_t digitalMinusAddr = 8;
			constexpr FactoryIO::modbusAddr_t analogAddr = 4;

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			bool readValue = 0;

			FactoryIO::Convayor_t convayor(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::DIGITAL_PLUS_MINUS);

			convayor.moveDirection(false);
			mb.modbus_read_coils(digitalPlusAddr, 1, &readValue);
			Assert::IsTrue(readValue, L"convayor didn't move forward");
			mb.modbus_read_coils(digitalMinusAddr, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor minus shouldn't turn on");

			convayor.moveDirection(true);
			mb.modbus_read_coils(digitalPlusAddr, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor plus shouldn't turn on");
			mb.modbus_read_coils(digitalMinusAddr, 1, &readValue);
			Assert::IsTrue(readValue, L"convayor didn't move forward");

			convayor.stop();
			mb.modbus_read_coils(digitalPlusAddr, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor didn't stop");
			mb.modbus_read_coils(digitalMinusAddr, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor didn't stop");

			mb.modbus_close();
		}

		TEST_METHOD(analog) {
			constexpr FactoryIO::modbusAddr_t digitalAddr = 6;
			constexpr FactoryIO::modbusAddr_t digitalPlusAddr = 7;
			constexpr FactoryIO::modbusAddr_t digitalMinusAddr = 8;
			constexpr FactoryIO::modbusAddr_t analogAddr = 4;

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			uint16_t readValue = 0;

			FactoryIO::Convayor_t convayor(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::ANALOG);

			for (float testValue = -1; testValue < 1; testValue = testValue + 0.1f) {
				convayor.moveAtSpeed(testValue);
				mb.modbus_read_holding_registers(analogAddr, 1, &readValue);
				Assert::AreEqual(static_cast<uint16_t>(FactoryIO::map(testValue, -1.0f, 1.0f, -10.0f, 10.0f)), readValue, L"should speed is not equal to is speed");
			}

			convayor.stop();
			mb.modbus_read_holding_registers(analogAddr, 1, &readValue);
			Assert::AreEqual((uint16_t)0, readValue, L"convayorbelt didn't stop");

			convayor.move(true);
			mb.modbus_read_holding_registers(analogAddr, 1, &readValue);
			Assert::AreEqual((uint16_t)10, readValue, L"convayorbelt didn't start");

			convayor.move(false);
			mb.modbus_read_holding_registers(analogAddr, 1, &readValue);
			Assert::AreEqual((uint16_t)0, readValue, L"convayorbelt didn't stop");

			mb.modbus_close();
		}

		TEST_METHOD(exceptions) {
			constexpr FactoryIO::modbusAddr_t digitalAddr = 6;
			constexpr FactoryIO::modbusAddr_t digitalPlusAddr = 7;
			constexpr FactoryIO::modbusAddr_t digitalMinusAddr = 8;
			constexpr FactoryIO::modbusAddr_t analogAddr = 4;

			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::ConvayorMode_t::DIGITAL);
				}, L"convayor constructior doesn't throw error when in digital mode");
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::ConvayorMode_t::DIGITAL_PLUS_MINUS);
				}, L"convayor constructior doesn't throw error when in digital plus minus mode");
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::ConvayorMode_t::ANALOG);
				}, L"convayor constructior doesn't throw error when in analog mode");

			FactoryIO::Convayor_t convayorDigital(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::DIGITAL);
			FactoryIO::Convayor_t convayorDigitalPlusMinus(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::DIGITAL_PLUS_MINUS);
			FactoryIO::Convayor_t convayorAnalog(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::ANALOG);

			Assert::ExpectException<std::domain_error, std::function<void(void)>>([&]() {
				convayorDigital.moveDirection(true);
				}, L"moveDirection doesn't throw error in wrong mode");

			Assert::ExpectException<std::domain_error, std::function<void(void)>>([&]() {
				convayorDigital.moveAtSpeed(0);
				}, L"moveAtSpeed doesn't throw error in wrong mode");

			Assert::ExpectException<std::domain_error, std::function<void(void)>>([&]() {
				convayorDigitalPlusMinus.moveAtSpeed(0);
				}, L"moveAtSpeed doesn't throw error in wrong mode");

			Assert::ExpectException<std::out_of_range, std::function<void(void)>>([&]() {
				convayorAnalog.moveAtSpeed(-2.0f);
				}, L"convayorAnalog.moveAtSpeed doens't cause exception when value is out of bounds");
			Assert::ExpectException<std::out_of_range, std::function<void(void)>>([&]() {
				convayorAnalog.moveAtSpeed(2.0f);
				}, L"convayorAnalog.moveAtSpeed doens't cause exception when value is out of bounds");

			Assert::ExpectException<std::domain_error, std::function<void(void)>>([&]() {
				convayorAnalog.moveDirection(true);
				}, L"moveDirection doesn't throw error in wrong mode");

		}
	};
}


namespace _2FactoryIOLibTest_integration {
	TEST_CLASS(emmiterRemover) {
		TEST_METHOD(spawningDeleting) {
			modbus mb = modbus("127.0.0.1", 502);
			mb.modbus_set_slave_id(1);
			if (!mb.modbus_connect()) {
				Assert::Fail(L"Couldn't connect to FactoryIO");
			}

			FactoryIO::remover_t remover(mb, 3, 2, 1, 0);
			FactoryIO::emmiter_t emmiter(mb, 5, 1, 0);

			emmiter.emmit(false);
			remover.setBasesToRemove(FactoryIO::presets::allBases);
			remover.setPartsToRemove(FactoryIO::presets::allParts);
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			Assert::AreEqual((size_t)0, remover.getDetectedParts().size(), L"Remover didn't remove all parts");
			Assert::AreEqual((size_t)0, remover.getDetectedBases().size(), L"Remover didn't remove all bases");
			remover.setBasesToRemove({});
			remover.setPartsToRemove({});

			emmiter.setBase({});
			for (size_t i = 1; i < 14; i++) {
				emmiter.setParts(FactoryIO::internal::BitfieldEnumMapper_t::toParts((uint16_t)(1 << i)));
				emmiter.emmit(true);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				emmiter.emmit(false);
				Assert::AreEqual((uint16_t)(1 << i), FactoryIO::internal::BitfieldEnumMapper_t::toBitfield(remover.getDetectedParts()), L"missmatch in element detection / emission");
				
				remover.setPartsToRemove(FactoryIO::internal::BitfieldEnumMapper_t::toParts((uint16_t)(1 << i)));
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				Assert::AreEqual((size_t)0, remover.getDetectedParts().size(), L"Remover didn't remove all parts");
				remover.setPartsToRemove({});
			}
			emmiter.setParts({});

			for (size_t i = 1; i < 3; i++) {
				emmiter.setBase(FactoryIO::internal::BitfieldEnumMapper_t::toBases((uint16_t)(1 << i)));
				emmiter.emmit(true);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				emmiter.emmit(false);
				Assert::AreEqual((uint16_t)(1 << i), FactoryIO::internal::BitfieldEnumMapper_t::toBitfield(remover.getDetectedBases()), L"missmatch in element detection / emission");

				remover.setBasesToRemove(FactoryIO::internal::BitfieldEnumMapper_t::toBases((uint16_t)(1 << i)));
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				Assert::AreEqual((size_t)0, remover.getDetectedBases().size(), L"Remover didn't remove all parts");
				remover.setBasesToRemove({});
			}
			emmiter.setBase({});
		}

	};
}