#include "Encoder.h"
#include <stdexcept>
#include <iostream>

FactoryIO::encoder_t::encoder_t(ModbusProvider_t& _mb, modbusAddr_t signalA, modbusAddr_t signalB) :
	encoder_t(_mb, signalA, signalB, true) { }

FactoryIO::encoder_t::encoder_t(ModbusProvider_t& _mb, modbusAddr_t signalA, modbusAddr_t signalB, bool startThreadImmidiantly) : 
	_signalAAddr(signalA),
	_signalBAddr(signalB),
	_mb(_mb),
	_privateModbus(_mb.getModbus()) {
	_threadWait.store(!startThreadImmidiantly);
	FactoryIO::internal::checkModbusAddr(signalA);
	FactoryIO::internal::checkModbusAddr(signalB);

	_updateThread = std::thread(&FactoryIO::encoder_t::_updateSignal, this);
}

FactoryIO::encoder_t::~encoder_t() {
	_treadRun = false;
	while (!_updateThread.joinable())
		;
	_updateThread.join();
}

void FactoryIO::encoder_t::_updateSignal() {
	bool _signalA = false;
	bool _signalB = false;

	byte _thisPhase = 0;
	byte _oldPhase = 0;

	int tmpRotationCounter = 0;

	_privateModbus.modbus_connect();
	if (!_privateModbus.is_connected())
		throw std::runtime_error("thread couldn't connect to modbus server.");

	_privateModbus.modbus_read_input_bits(_signalAAddr, 1, &_signalA);
	_privateModbus.modbus_read_input_bits(_signalBAddr, 1, &_signalB);
	_oldPhase = getPhase(_signalA, _signalB);

	uint16_t errorCounter = 0;

	while (_treadRun.load()) {
		_privateModbus.modbus_read_input_bits(_signalAAddr, 1, &_signalA);
		_privateModbus.modbus_read_input_bits(_signalBAddr, 1, &_signalB);

		_thisPhase = getPhase(_signalA, _signalB);

		if (_thisPhase != _oldPhase) {
			if (_thisPhase == phaseIncrement(_oldPhase)) {
				tmpRotationCounter = _rotationCounter.load();
				++tmpRotationCounter;
				_rotationCounter.store(tmpRotationCounter);
				errorCounter = 0;
			}
			else if (_thisPhase == phaseDecrement(_oldPhase)) {
				tmpRotationCounter = _rotationCounter.load();
				--tmpRotationCounter;
				_rotationCounter.store(tmpRotationCounter);
				errorCounter = 0;
			}
			else {
				errorCounter++;
			}
			
			if (errorCounter > _allowedMisses.load())
				_lostTrackOfEncoder.store(true);

			while (_lostTrackOfEncoder.load() && _treadRun.load()) {
				// wait for removeing of error or termination of thread
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}

			while (_threadWait.load() && _treadRun.load()) {
				// wait for thread to be resumed
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			_oldPhase = _thisPhase;
			//Concurrency::diagnostic::span s(marker_series, L"cycle complete");
			
		}
		precise_sleep_ns(std::chrono::duration_cast<std::chrono::nanoseconds>(_updateTime.load()).count());
	}
}

byte FactoryIO::encoder_t::getPhase(bool A, bool B) {
	byte tmp = ((byte)A << 1) | (byte)B;
	if (tmp == 3)
		return 3;
	if (tmp == 2)
		return 4;
	return tmp + 1;
}

inline byte FactoryIO::encoder_t::phaseIncrement(byte phase) {
	byte _phase = phase + 1;
	if (_phase == 5)
		return 1;
	return _phase;
}

inline byte FactoryIO::encoder_t::phaseDecrement(byte phase) {
	byte _phase = phase - 1;
	if (_phase == 0)
		return 4;
	return _phase;
}

void FactoryIO::encoder_t::hybrid_sleep(std::chrono::nanoseconds duration) {
	using clock = std::chrono::high_resolution_clock;
	auto target = clock::now() + duration;
	auto wakeup = target - std::chrono::microseconds(200);
	std::this_thread::sleep_until(wakeup);
	while (clock::now() < target) {
		// fine-tune with busy wait
	}
}

void FactoryIO::encoder_t::precise_sleep_ns(long long nanoseconds) {
	auto start = std::chrono::high_resolution_clock::now();
	auto end = start + std::chrono::nanoseconds(nanoseconds);
	while (std::chrono::high_resolution_clock::now() < end) {
		// spin
	}
}