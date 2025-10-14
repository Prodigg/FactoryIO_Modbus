#include "convayor.h"
#include <stdexcept>

FactoryIO::Convayor_t::Convayor_t(modbus& mb, modbusAddr_t digital, modbusAddr_t digitalPlus, modbusAddr_t digitalMinus, modbusAddr_t analog, ConvayorMode_t mode) : 
	mb(mb),
	_digitalAddr(digital), 
	_digitalPlusAddr(digitalPlus), 
	_digitalMinusAddr(digitalMinus), 
	_analogAddr(analog),
	_mode(mode) { 
	if (_mode == ConvayorMode_t::DIGITAL && _digitalAddr == NO_MODBUS_ADDR)
		throw std::runtime_error("unable to generate digital convayor with no digital modbus adress");
	if (_mode == ConvayorMode_t::DIGITAL_PLUS_MINUS && _digitalPlusAddr == NO_MODBUS_ADDR && _digitalMinusAddr == NO_MODBUS_ADDR)
		throw std::runtime_error("unable to generate digital plus minus convayor with no digital plus and minus modbus adress");
	if (_mode == ConvayorMode_t::ANALOG && _analogAddr == NO_MODBUS_ADDR)
		throw std::runtime_error("unable to generate analog convayor with no analog modbus adress");
}

void FactoryIO::Convayor_t::move(bool move) {
	switch (_mode) {
	case FactoryIO::ConvayorMode_t::DIGITAL:
		FactoryIO::internal::checkModbusAddr(_digitalAddr);
		mb.modbus_write_coil(_digitalAddr, move);
		break;
	case FactoryIO::ConvayorMode_t::DIGITAL_PLUS_MINUS:
		FactoryIO::internal::checkModbusAddr(_digitalPlusAddr);
		FactoryIO::internal::checkModbusAddr(_digitalMinusAddr);

		mb.modbus_write_coil(_digitalMinusAddr, false);
		mb.modbus_write_coil(_digitalPlusAddr, move);
		break;
	case FactoryIO::ConvayorMode_t::ANALOG:
		if (move)
			moveAtSpeed(1.0f);
		else
			moveAtSpeed(0);
		break;
	default:
		throw std::runtime_error("invalid state of convayor.");
		break;
	}
}

void FactoryIO::Convayor_t::moveDirection(bool reverse) {
	if (_mode != ConvayorMode_t::DIGITAL_PLUS_MINUS)
		throw std::domain_error("Convayor is not in digital plus/minus mode");
	FactoryIO::internal::checkModbusAddr(_digitalPlusAddr);
	FactoryIO::internal::checkModbusAddr(_digitalMinusAddr);

	mb.modbus_write_coil(_digitalPlusAddr, !reverse);
	mb.modbus_write_coil(_digitalMinusAddr, reverse);
}

void FactoryIO::Convayor_t::moveAtSpeed(float speed) {
	if (_mode != ConvayorMode_t::ANALOG)
		throw std::domain_error("Convayor is not in analog mode");
	FactoryIO::internal::checkModbusAddr(_analogAddr);
	if (speed < -1 || speed > 1)
		throw std::out_of_range("speed must be in range 1, -1 (range out of bounds)");
	
	mb.modbus_write_register(_analogAddr, static_cast<int16_t>(FactoryIO::map(speed, -1.0f, 1.0f, -10.0f, 10.0f)));

}

void FactoryIO::Convayor_t::stop() {
	switch (_mode) {
	case FactoryIO::ConvayorMode_t::DIGITAL:
		FactoryIO::internal::checkModbusAddr(_digitalAddr);
		mb.modbus_write_coil(_digitalAddr, false);
		break;
	case FactoryIO::ConvayorMode_t::DIGITAL_PLUS_MINUS:
		FactoryIO::internal::checkModbusAddr(_digitalPlusAddr);
		FactoryIO::internal::checkModbusAddr(_digitalMinusAddr);

		mb.modbus_write_coil(_digitalMinusAddr, false);
		mb.modbus_write_coil(_digitalPlusAddr, false);
		break;
	case FactoryIO::ConvayorMode_t::ANALOG:
		moveAtSpeed(0);
		break;
	default:
		throw std::runtime_error("invalid state of convayor.");
		break;
	}
}
