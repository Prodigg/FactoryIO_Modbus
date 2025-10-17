#include <FactoryIO.h>
#include <iostream>

int main() {
	constexpr FactoryIO::modbusAddr_t analogAddr = 4;
	constexpr FactoryIO::modbusAddr_t signalAAddr = 0;
	constexpr FactoryIO::modbusAddr_t signalBAddr = 1;
	constexpr uint16_t scaleFactor = 100;

	modbus mb = modbus("127.0.0.1", 502);
	mb.modbus_set_slave_id(1);
	if (!mb.modbus_connect()) {
		exit(EXIT_FAILURE);
	}
	FactoryIO::Convayor_t convayor(mb, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, FactoryIO::NO_MODBUS_ADDR, analogAddr, FactoryIO::ConvayorMode_t::ANALOG, scaleFactor);
	FactoryIO::encoder_t encoder(mb, signalAAddr, signalBAddr);

	encoder.setRotationCounter(0);
	encoder.setAllowedMisses(10);
	encoder.setUpdateCycleTime(std::chrono::milliseconds(1));
	convayor.moveAtSpeed(1.0f);
	std::this_thread::sleep_for(std::chrono::seconds(10));
	convayor.moveAtSpeed(0.0f);
	
	mb.modbus_close();
}