#include "convayorScale.h"

FactoryIO::convayorScale_t::convayorScale_t(
  ModbusProvider_t& mb, 
  modbusAddr_t digitalPlus, 
  modbusAddr_t digitalMinus, 
  modbusAddr_t currentWeight, 
  uint16_t scaleFactor) : 
  Convayor_t(
    mb, 
    NO_MODBUS_ADDR, 
    digitalPlus, 
    digitalMinus, 
    NO_MODBUS_ADDR, 
    ConvayorMode_t::DIGITAL_PLUS_MINUS,
    scaleFactor), 
   _currentWeightIndex(currentWeight),
   _scaleFactor(scaleFactor) { }

double FactoryIO::convayorScale_t::getCurrentWeight() {
  FactoryIO::internal::checkModbusAddr(_currentWeightIndex);
  uint16_t currentWeight = mb.readInputRegister(_currentWeightIndex);
  return map<double>(currentWeight, 0.0, 10.0 * static_cast<float>(_scaleFactor), 0.0, 1);
}
