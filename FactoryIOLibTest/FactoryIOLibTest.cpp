#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace FactoryIO::internal::testing;

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(alarmSirene) {
	TEST_METHOD(turnOnOff) {
		FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);
		
		FactoryIO::alarmSiren_t alarmSirene(mb, 0);
		bool ModbusServerSireneOn = false;

		///// check if sirene turnes on

		alarmSirene.setSireneState(true);

		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		mb.getModbus().modbus_read_coils(0, 1, &ModbusServerSireneOn);
		Assert::AreEqual(true, ModbusServerSireneOn, L"sirene didn't turn on");

		///// check if sirene turnes off

		alarmSirene.setSireneState(false);

		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		mb.getModbus().modbus_read_coils(0, 1, &ModbusServerSireneOn);
		Assert::AreEqual(false, ModbusServerSireneOn, L"sirene didn't turn off");
	}

	TEST_METHOD(noAddress) {
		FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);
		
		FactoryIO::alarmSiren_t alarmSirene(mb, FactoryIO::NO_MODBUS_ADDR);
		Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
			alarmSirene.setSireneState(true);
			}, L"setSireneState did not rase a exception when no valid modbus addr is provided.");
	}
	};

	TEST_CLASS(warningLight) {
		TEST_METHOD(turnOnOff) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::WarningLight_t warningLight(mb, 1);
			bool ModbusServerLightOn = false;

			///// check if light turnes on

			warningLight.setLightState(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			mb.getModbus().modbus_read_coils(1, 1, &ModbusServerLightOn);
			Assert::AreEqual(true, ModbusServerLightOn, L"sirene didn't turn on");

			///// check if light turnes off

			warningLight.setLightState(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			mb.getModbus().modbus_read_coils(0, 1, &ModbusServerLightOn);
			Assert::AreEqual(false, ModbusServerLightOn, L"sirene didn't turn off");
		}

		TEST_METHOD(noAddress) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::WarningLight_t warningLight(mb, FactoryIO::NO_MODBUS_ADDR);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				warningLight.setLightState(true);
				}, L"setSireneState did not rase a exception when no valid modbus addr is provided.");

		}
	};

	TEST_CLASS(stackLight) {
		TEST_METHOD(greenLight) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::StackLight_t stackLight(mb, 2, 3, 4);

			///// check if green light turnes on

			stackLight.setGreenLight(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		
			Assert::AreEqual(true, getModbusCoilState(2, mb.getModbus()), L"green light didn't turn on");
			Assert::AreEqual(false, getModbusCoilState(3, mb.getModbus()), L"orange light turned on");
			Assert::AreEqual(false, getModbusCoilState(4, mb.getModbus()), L"red light turned on");

			///// check if light turnes off

			stackLight.setGreenLight(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb.getModbus()), L"green light didn't turn off");
			Assert::AreEqual(false, getModbusCoilState(3, mb.getModbus()), L"orange light turned on");
			Assert::AreEqual(false, getModbusCoilState(4, mb.getModbus()), L"red light turned on");

		}

		TEST_METHOD(orangeLight) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::StackLight_t stackLight(mb, 2, 3, 4);

			///// check if green light turnes on

			stackLight.setOrangeLight(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb.getModbus()), L"green light turned on");
			Assert::AreEqual(true, getModbusCoilState(3, mb.getModbus()), L"orange light didn't turn on");
			Assert::AreEqual(false, getModbusCoilState(4, mb.getModbus()), L"red light turned on");

			///// check if light turnes off

			stackLight.setOrangeLight(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb.getModbus()), L"green light turned on");
			Assert::AreEqual(false, getModbusCoilState(3, mb.getModbus()), L"orange light didn't turn off");
			Assert::AreEqual(false, getModbusCoilState(4, mb.getModbus()), L"red light turned on");

		}

		TEST_METHOD(RedLight) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::StackLight_t stackLight(mb, 2, 3, 4);

			///// check if green light turnes on

			stackLight.setRedLight(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb.getModbus()), L"green light turned on");
			Assert::AreEqual(false, getModbusCoilState(3, mb.getModbus()), L"orange light turned on");
			Assert::AreEqual(true, getModbusCoilState(4, mb.getModbus()), L"red light didn't turn on");

			///// check if light turnes off

			stackLight.setRedLight(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(20));

			Assert::AreEqual(false, getModbusCoilState(2, mb.getModbus()), L"green light turned on");
			Assert::AreEqual(false, getModbusCoilState(3, mb.getModbus()), L"orange light turned on");
			Assert::AreEqual(false, getModbusCoilState(4, mb.getModbus()), L"red light didn't turn off");

		}

		TEST_METHOD(noAddress) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

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

		}
	};

	TEST_CLASS(emmiter) {
		TEST_METHOD(emmitState) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::emmiter_t emmiter(mb, 5, 1, 0);

			emmiter.emmit(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual(true, getModbusCoilState(5, mb.getModbus()), L"emmit didn't turn on");

			emmiter.emmit(false);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual(false, getModbusCoilState(5, mb.getModbus()), L"emmit didn't turn off");

		}

		TEST_METHOD(emmitBase) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::emmiter_t emmiter(mb, 5, 1, 0);

			emmiter.emmit(true);
			emmiter.setBase({ FactoryIO::Bases_t::PALLET });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual(true, getModbusCoilState(5, mb.getModbus()), L"emmit didn't turn on");
			Assert::AreEqual((uint16_t)0b00000000000000001, getModbusRegState(0, mb.getModbus()), L"Pallet bit didn't turn on");

			emmiter.setBase({ FactoryIO::Bases_t::PALLET, FactoryIO::Bases_t::SQARE_PALLET });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000011, getModbusRegState(0, mb.getModbus()), L"emmiter base missmatch");

			emmiter.setBase({ FactoryIO::Bases_t::PALLET, FactoryIO::Bases_t::SQARE_PALLET, FactoryIO::Bases_t::STACKABLE_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000111, getModbusRegState(0, mb.getModbus()), L"emmiter base missmatch");

			emmiter.setBase({ FactoryIO::Bases_t::NO_BASE });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000000, getModbusRegState(0, mb.getModbus()), L"emmiter base didn't clear");

			emmiter.emmit(false);
		}

		TEST_METHOD(emmitPart) {
			using namespace FactoryIO;
			ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::emmiter_t emmiter(mb, 5, 1, 0);

			emmiter.emmit(true);
			emmiter.setParts({ Parts_t::SMALL_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual(true, getModbusCoilState(5, mb.getModbus()), L"emmit didn't turn on");
			Assert::AreEqual((uint16_t)0b00000000000000001, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX, Parts_t::MEDIUM_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000011, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX, Parts_t::MEDIUM_BOX, Parts_t::LARGE_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000000111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX, Parts_t::MEDIUM_BOX, Parts_t::LARGE_BOX, Parts_t::PALLETIZING_BOX });
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000001111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX, 
				Parts_t::MEDIUM_BOX, 
				Parts_t::LARGE_BOX, 
				Parts_t::PALLETIZING_BOX, 
				Parts_t::BLUE_RAW_MATERIAL 
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000011111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000000111111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

			emmiter.setParts({ FactoryIO::Parts_t::SMALL_BOX,
				Parts_t::MEDIUM_BOX,
				Parts_t::LARGE_BOX,
				Parts_t::PALLETIZING_BOX,
				Parts_t::BLUE_RAW_MATERIAL,
				Parts_t::GREEN_RAW_MATERIAL,
				Parts_t::METTAL_RAW_MATERIAL
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			Assert::AreEqual((uint16_t)0b00000000001111111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

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
			Assert::AreEqual((uint16_t)0b00000000011111111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

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
			Assert::AreEqual((uint16_t)0b00000000111111111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

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
			Assert::AreEqual((uint16_t)0b00000001111111111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

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
			Assert::AreEqual((uint16_t)0b00000011111111111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

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
			Assert::AreEqual((uint16_t)0b00000111111111111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

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
			Assert::AreEqual((uint16_t)0b00001111111111111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

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
			Assert::AreEqual((uint16_t)0b00011111111111111, getModbusRegState(1, mb.getModbus()), L"emmiter part missmatch");

			emmiter.emmit(false);
		}

		TEST_METHOD(noAddress) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

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
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::remover_t remover(mb, 3, 2, 1, 0);

			uint16_t returnBitfield = 0;
			uint16_t modbusReadValue = 0;

			for (uint16_t toTestBitfield = 0; toTestBitfield < 0b0000000000000111; toTestBitfield++) {
				remover.setBasesToRemove(FactoryIO::internal::BitfieldEnumMapper_t::toBases(toTestBitfield));

				mb.getModbus().modbus_read_holding_registers(2, 1, &modbusReadValue);
				Assert::AreEqual((uint16_t)toTestBitfield, (uint16_t)modbusReadValue, L"bitfield doesn't match with expected bitfield.");
			}

			remover.setBasesToRemove({});
		}

		TEST_METHOD(toRemovePart) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::remover_t remover(mb, 3, 2, 1, 0);

			uint16_t returnBitfield = 0;
			uint16_t modbusReadValue = 0;

			for (uint16_t toTestBitfield = 0; toTestBitfield < 0b0011111111111111; toTestBitfield++) {
				remover.setPartsToRemove(FactoryIO::internal::BitfieldEnumMapper_t::toParts(toTestBitfield));

				mb.getModbus().modbus_read_holding_registers(3, 1, &modbusReadValue);
				Assert::AreEqual((uint16_t)toTestBitfield, (uint16_t)modbusReadValue, L"bitfield doesn't match with expected bitfield.");
			}
			remover.setPartsToRemove({});
		}

		TEST_METHOD(noAdress) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

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
		}
	};

	TEST_CLASS(Convayor) {
		TEST_METHOD(digital) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			bool readValue = 0;

			FactoryIO::Convayor_t convayor(mb, 6, 7, 8, 4, FactoryIO::ConvayorMode_t::DIGITAL, 0);
			convayor.move(true);
			mb.getModbus().modbus_read_coils(6, 1, &readValue);
			Assert::IsTrue(readValue, L"convayor didn't start moving.");

			convayor.move(false);
			mb.getModbus().modbus_read_coils(6, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor didn't stop moving.");

			convayor.move(true);
			mb.getModbus().modbus_read_coils(6, 1, &readValue);
			Assert::IsTrue(readValue, L"convayor didn't start moving.");

			convayor.stop();
			mb.getModbus().modbus_read_coils(6, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor didn't stop moving.");
		}

		TEST_METHOD(digitalPlusMinus) {
			constexpr FactoryIO::modbusAddr_t digitalAddr = 6;
			constexpr FactoryIO::modbusAddr_t digitalPlusAddr = 7;
			constexpr FactoryIO::modbusAddr_t digitalMinusAddr = 8;
			constexpr FactoryIO::modbusAddr_t analogAddr = 4;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			bool readValue = 0;

			FactoryIO::Convayor_t convayor(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::DIGITAL_PLUS_MINUS, 0);

			convayor.moveDirection(false);
			mb.getModbus().modbus_read_coils(digitalPlusAddr, 1, &readValue);
			Assert::IsTrue(readValue, L"convayor didn't move forward");
			mb.getModbus().modbus_read_coils(digitalMinusAddr, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor minus shouldn't turn on");

			convayor.moveDirection(true);
			mb.getModbus().modbus_read_coils(digitalPlusAddr, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor plus shouldn't turn on");
			mb.getModbus().modbus_read_coils(digitalMinusAddr, 1, &readValue);
			Assert::IsTrue(readValue, L"convayor didn't move forward");

			convayor.stop();
			mb.getModbus().modbus_read_coils(digitalPlusAddr, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor didn't stop");
			mb.getModbus().modbus_read_coils(digitalMinusAddr, 1, &readValue);
			Assert::IsFalse(readValue, L"convayor didn't stop");
		}

		TEST_METHOD(analog) {
			constexpr FactoryIO::modbusAddr_t digitalAddr = 6;
			constexpr FactoryIO::modbusAddr_t digitalPlusAddr = 7;
			constexpr FactoryIO::modbusAddr_t digitalMinusAddr = 8;
			constexpr FactoryIO::modbusAddr_t analogAddr = 4;
			constexpr uint16_t scaleFactor = 100;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			uint16_t readValue = 0;

			FactoryIO::Convayor_t convayor(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::ANALOG, scaleFactor);

			for (float testValue = -1; testValue < 1; testValue = testValue + 0.1f) {
				convayor.moveAtSpeed(testValue);
				mb.getModbus().modbus_read_holding_registers(analogAddr, 1, &readValue);
				Assert::AreEqual(static_cast<uint16_t>(FactoryIO::map(testValue, -1.0f, 1.0f, -10.0f, 10.0f) * scaleFactor), readValue, L"should speed is not equal to is speed");
			}

			convayor.stop();
			mb.getModbus().modbus_read_holding_registers(analogAddr, 1, &readValue);
			Assert::AreEqual((uint16_t)0, readValue, L"convayorbelt didn't stop");

			convayor.move(true);
			mb.getModbus().modbus_read_holding_registers(analogAddr, 1, &readValue);
			Assert::AreEqual((uint16_t)1000, readValue, L"convayorbelt didn't start");

			convayor.move(false);
			mb.getModbus().modbus_read_holding_registers(analogAddr, 1, &readValue);
			Assert::AreEqual((uint16_t)0, readValue, L"convayorbelt didn't stop");
		}

		TEST_METHOD(exceptions) {
			constexpr FactoryIO::modbusAddr_t digitalAddr = 6;
			constexpr FactoryIO::modbusAddr_t digitalPlusAddr = 7;
			constexpr FactoryIO::modbusAddr_t digitalMinusAddr = 8;
			constexpr FactoryIO::modbusAddr_t analogAddr = 4;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::ConvayorMode_t::DIGITAL, 0);
				}, L"convayor constructior doesn't throw error when in digital mode");
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::ConvayorMode_t::DIGITAL_PLUS_MINUS, 0);
				}, L"convayor constructior doesn't throw error when in digital plus minus mode");
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>([&]() {
				FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::ConvayorMode_t::ANALOG, 0);
				}, L"convayor constructior doesn't throw error when in analog mode");

			FactoryIO::Convayor_t convayorDigital(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::DIGITAL, 0);
			FactoryIO::Convayor_t convayorDigitalPlusMinus(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::DIGITAL_PLUS_MINUS, 0);
			FactoryIO::Convayor_t convayorAnalog(mb, digitalAddr, digitalPlusAddr, digitalMinusAddr, analogAddr, FactoryIO::ConvayorMode_t::ANALOG, 0);

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
	TEST_CLASS(Encoder) {
		TEST_METHOD(forward) {
			constexpr FactoryIO::modbusAddr_t analogAddr = 4;
			constexpr FactoryIO::modbusAddr_t signalAAddr = 0;
			constexpr FactoryIO::modbusAddr_t signalBAddr = 1;
			constexpr uint16_t scaleFactor = 100;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, analogAddr, FactoryIO::ConvayorMode_t::ANALOG, scaleFactor);
			FactoryIO::encoder_t encoder(mb, signalAAddr, signalBAddr);

			encoder.setRotationCounter(0);
			encoder.setUpdateCycleTime(std::chrono::milliseconds(10));
			convayor.moveAtSpeed(0.15f);
			std::this_thread::sleep_for(std::chrono::seconds(10));
			convayor.moveAtSpeed(0.0f);
			Logger::WriteMessage(const_cast<const char*>(std::string("rotations: " + encoder.getRotationCounter()).c_str()));
			Assert::IsTrue(encoder.isPositionValid(), L"encoder lost position");
			Assert::IsTrue(0 < encoder.getRotationCounter(), L"convayor didn't move forward");
			
		}

		TEST_METHOD(reverse) {
			constexpr FactoryIO::modbusAddr_t analogAddr = 4;
			constexpr FactoryIO::modbusAddr_t signalAAddr = 0;
			constexpr FactoryIO::modbusAddr_t signalBAddr = 1;
			constexpr uint16_t scaleFactor = 100;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, analogAddr, FactoryIO::ConvayorMode_t::ANALOG, scaleFactor);
			FactoryIO::encoder_t encoder(mb, signalAAddr, signalBAddr);

			encoder.setRotationCounter(0);
			encoder.setUpdateCycleTime(std::chrono::milliseconds(2));
			convayor.moveAtSpeed(-0.15f);
			std::this_thread::sleep_for(std::chrono::seconds(10));
			convayor.moveAtSpeed(0.0f);
			Logger::WriteMessage(const_cast<const char*>(std::string("rotations: " + encoder.getRotationCounter()).c_str()));
			Assert::IsTrue(encoder.isPositionValid(), L"encoder lost position");
			Assert::IsTrue(0 > encoder.getRotationCounter(), L"convayor didn't move backward");

		}

		TEST_METHOD(excetions) {
			constexpr FactoryIO::modbusAddr_t signalAAddr = 0;
			constexpr FactoryIO::modbusAddr_t signalBAddr = 1;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::encoder_t encoder(mb, signalAAddr, FactoryIO::NO_MODBUS_ADDR);
				}, L"no excetion when no modbusAdress is provided in signalA"
			);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::encoder_t encoder(mb, FactoryIO::NO_MODBUS_ADDR, signalBAddr);
				}, L"no excetion when no modbusAdress is provided in signalA"
			);

		}

		TEST_METHOD(highspeed) {
			constexpr FactoryIO::modbusAddr_t analogAddr = 4;
			constexpr FactoryIO::modbusAddr_t signalAAddr = 0;
			constexpr FactoryIO::modbusAddr_t signalBAddr = 1;
			constexpr uint16_t scaleFactor = 100;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, analogAddr, FactoryIO::ConvayorMode_t::ANALOG, scaleFactor);
			FactoryIO::encoder_t encoder(mb, signalAAddr, signalBAddr, false);

			encoder.setRotationCounter(0);
			encoder.setUpdateCycleTime(std::chrono::milliseconds(0));
			encoder.setAllowedMisses(3);
			encoder.resumeThread();
			convayor.moveAtSpeed(1.0f);
			std::this_thread::sleep_for(std::chrono::seconds(10));
			convayor.moveAtSpeed(0.0f);
			encoder.pauseThread();
			Logger::WriteMessage(const_cast<const char*>(std::string("rotations: " + encoder.getRotationCounter()).c_str()));
			Assert::IsTrue(encoder.isPositionValid(), L"encoder lost position");
			Assert::IsTrue(0 < encoder.getRotationCounter(), L"convayor didn't move forward");

		}
	};

	TEST_CLASS(rollerStop) {
		TEST_METHOD(setRollerStop) {
			constexpr FactoryIO::modbusAddr_t rollerStopAddr = 9;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			bool modbusReadValue = false;

			FactoryIO::rollerStop_t rollerStop(mb, rollerStopAddr);

			rollerStop.setRollerState(true);
			mb.getModbus().modbus_read_coils(rollerStopAddr, 1, &modbusReadValue);
			Assert::IsTrue(modbusReadValue, L"roller isn't active");

			rollerStop.setRollerState(false);
			mb.getModbus().modbus_read_coils(rollerStopAddr, 1, &modbusReadValue);
			Assert::IsFalse(modbusReadValue, L"roller isn't deactivated");
		}

		TEST_METHOD(exceptions) {
			constexpr FactoryIO::modbusAddr_t rollerStopAddr = 9;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::rollerStop_t rollerStop(mb, FactoryIO::NO_MODBUS_ADDR);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					rollerStop.setRollerState(true);
				}, L"no excetion when no modbusAdress is provided"
			);
		}
	};

	TEST_CLASS(chainTransfer) {
		TEST_METHOD(move) {
			constexpr FactoryIO::modbusAddr_t ChainTransferPlus = 10;
			constexpr FactoryIO::modbusAddr_t ChainTransferMinus = 11;
			constexpr FactoryIO::modbusAddr_t ChainTransferLeft = 12;
			constexpr FactoryIO::modbusAddr_t ChainTransferRight = 13;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			bool modbusReadValue = false;

			FactoryIO::chainTransfer_t chainTransfer(mb, ChainTransferPlus, ChainTransferMinus, ChainTransferLeft, ChainTransferRight);

			chainTransfer.moveForward();
			mb.getModbus().modbus_read_coils(ChainTransferPlus, 1, &modbusReadValue);
			Assert::IsTrue(modbusReadValue, L"not moving forward");

			chainTransfer.moveBackward();
			mb.getModbus().modbus_read_coils(ChainTransferMinus, 1, &modbusReadValue);
			Assert::IsTrue(modbusReadValue, L"not moving backward");

			chainTransfer.moveLeft();
			mb.getModbus().modbus_read_coils(ChainTransferLeft, 1, &modbusReadValue);
			Assert::IsTrue(modbusReadValue, L"not moving left");

			chainTransfer.moveRight();
			mb.getModbus().modbus_read_coils(ChainTransferRight, 1, &modbusReadValue);
			Assert::IsTrue(modbusReadValue, L"not moving right");

			chainTransfer.stop();
			mb.getModbus().modbus_read_coils(ChainTransferPlus, 1, &modbusReadValue);
			Assert::IsFalse(modbusReadValue, L"not stopped");
			mb.getModbus().modbus_read_coils(ChainTransferMinus, 1, &modbusReadValue);
			Assert::IsFalse(modbusReadValue, L"not stopped");
			mb.getModbus().modbus_read_coils(ChainTransferLeft, 1, &modbusReadValue);
			Assert::IsFalse(modbusReadValue, L"not stopped");
			mb.getModbus().modbus_read_coils(ChainTransferRight, 1, &modbusReadValue);
			Assert::IsFalse(modbusReadValue, L"not stopped");
		}

		TEST_METHOD(exceptions) {
			constexpr FactoryIO::modbusAddr_t ChainTransferPlus = 10;
			constexpr FactoryIO::modbusAddr_t ChainTransferMinus = 11;
			constexpr FactoryIO::modbusAddr_t ChainTransferLeft = 12;
			constexpr FactoryIO::modbusAddr_t ChainTransferRight = 13;

			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

			FactoryIO::chainTransfer_t chainTransfer(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR);

			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					chainTransfer.moveForward();
				}, L"no excetion when no modbusAdress is provided"
			);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					chainTransfer.moveBackward();
				}, L"no excetion when no modbusAdress is provided"
			);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					chainTransfer.moveLeft();
				}, L"no excetion when no modbusAdress is provided"
			);
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					chainTransfer.moveRight();
				}, L"no excetion when no modbusAdress is provided"
			);

			chainTransfer.stop();
		}
	};
}


namespace _2FactoryIOLibTest_integration {
	TEST_CLASS(emmiterRemover) {
		TEST_METHOD(spawningDeleting) {
			FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

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