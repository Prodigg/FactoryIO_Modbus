#include "convayorScale.h"

FactoryIO::convayorScale_t::convayorScale_t(
  modbus mb, 
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
   _currentWeightIndex(currentWeight) { }

double FactoryIO::convayorScale_t::getCurrentWeight() {
  FactoryIO::internal::checkModbusAddr(_currentWeightIndex);
  uint16_t currentWeight = 0;
  mb.modbus_read_input_registers(_currentWeightIndex, 1, &currentWeight);
  return map<double>(currentWeight, 0.0, 10.0, 0.0, 1);
}
