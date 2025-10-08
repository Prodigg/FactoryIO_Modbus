#include <chrono>
#include <thread>
#include <iostream>
#include "FactoryIO.h"

int main() {
	modbus mb = modbus("127.0.0.1", 502);
	mb.modbus_set_slave_id(1);
	if (!mb.modbus_connect()) {
		std::cout << "ERROR: couldn't connect\n";
		exit(EXIT_FAILURE);
	}
	FactoryIO::alarmSiren_t alarmSirene(mb, 0);
	std::chrono::steady_clock::time_point begin;
	bool ModbusServerSireneOn = false;

	///// check if sirene turnes on

	alarmSirene.setSireneState(true);

	begin = std::chrono::steady_clock::now();
	while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() >= 50) {

	}

	mb.modbus_read_coils(0, 1, &ModbusServerSireneOn);
	

	///// check if sirene turnes off

	alarmSirene.setSireneState(false);

	begin = std::chrono::steady_clock::now();
	while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() >= 50) {

	}
	mb.modbus_read_coils(0, 1, &ModbusServerSireneOn);
	
}
