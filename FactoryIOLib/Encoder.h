#pragma once
#include "FactoryIOGeneral.h"
#include <thread>
#include <atomic>
#include <chrono>

namespace FactoryIO {

	/*!
	 * @brief This class uses a seperate thread with a seperate modbus object to read the encoder.
	 */
	class encoder_t {
	public:
		encoder_t(modbus& _mb, modbusAddr_t signalA, modbusAddr_t signalB);
		encoder_t(modbus& _mb, modbusAddr_t signalA, modbusAddr_t signalB, bool startThreadImmidiantly);
		~encoder_t();
		int getRotationCounter() { return _rotationCounter.load(); }
		void setRotationCounter(int newValue) { _rotationCounter.store(newValue); _lostTrackOfEncoder.store(false); }

		void setUpdateCycleTime(std::chrono::milliseconds time) { _updateTime.store(time); }

		bool isPositionValid() { return !_lostTrackOfEncoder.load(); }

		void setAllowedMisses(uint16_t allowedMisses) { _allowedMisses.store(allowedMisses); }

		void pauseThread() { _threadWait.store(true); }
		void resumeThread() { _threadWait.store(false); }
		bool isThreadRunning() { return _threadWait.load(); }
	private:
		void _updateSignal();

		byte getPhase(bool A, bool B);

		inline byte phaseIncrement(byte phase);
		inline byte phaseDecrement(byte phase);

		void hybrid_sleep(std::chrono::nanoseconds duration);
		void precise_sleep_ns(long long nanoseconds);

		modbus& _mb;
		modbusAddr_t _signalAAddr; 
		modbusAddr_t _signalBAddr;
		
		std::atomic<bool> _lostTrackOfEncoder = false;
		std::atomic<int> _rotationCounter = 0; 
		std::atomic<std::chrono::milliseconds> _updateTime = std::chrono::milliseconds(100);
		std::atomic<bool> _treadRun = true;
		std::atomic<uint16_t> _allowedMisses = 1;
		std::atomic<bool> _threadWait = false;

		modbus _privateModbus;
		std::thread _updateThread;
	};
}