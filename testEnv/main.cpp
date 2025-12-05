#include <FactoryIO.h>
#include <iostream>

int main() {
	constexpr FactoryIO::modbusAddr_t turntableRollPlus = 3;
	constexpr FactoryIO::modbusAddr_t turntableRollMinus = 4;
	constexpr FactoryIO::modbusAddr_t turntableFrontLimit = 3;
	constexpr FactoryIO::modbusAddr_t turntableBackLimit = 6;
	constexpr FactoryIO::modbusAddr_t turntableLimit0 = 4;
	constexpr FactoryIO::modbusAddr_t turntableLimit90 = 5;
	constexpr FactoryIO::modbusAddr_t turntableTurn = 2;
	constexpr FactoryIO::modbusAddr_t turntableTurnPlus = FactoryIO::NO_MODBUS_ADDR;
	constexpr FactoryIO::modbusAddr_t turntableTurnMinus = FactoryIO::NO_MODBUS_ADDR;

	FactoryIO::ModbusProvider_t mb("127.0.0.1", 502, 1);

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

	turntable.pickupBack().transferLeft();

	while (true) {
		turntable.update();
	}

	/*
	FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, analogAddr, FactoryIO::ConvayorMode_t::ANALOG, scaleFactor);
	FactoryIO::encoder_t encoder(mb, signalAAddr, signalBAddr);

	encoder.setRotationCounter(0);
	encoder.setAllowedMisses(10);
	encoder.setUpdateCycleTime(std::chrono::milliseconds(1));
	convayor.moveAtSpeed(1.0f);
	std::this_thread::sleep_for(std::chrono::seconds(10));
	convayor.moveAtSpeed(0.0f);
	*/
}