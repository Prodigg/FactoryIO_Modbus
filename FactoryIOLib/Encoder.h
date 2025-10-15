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
		~encoder_t();
		int getRotationCounter() { return _rotationCounter.load(); }
		void setRotationCounter(int newValue) { _rotationCounter.store(newValue); _lostTrackOfEncoder.store(false); }

		void setUpdateCycleTime(std::chrono::milliseconds time) { _updateTime.store(time); }

		bool isPositionValid() { return !_lostTrackOfEncoder.load(); }
	private:
		void _updateSignal();

		byte getPhase(bool A, bool B);

		inline byte phaseIncrement(byte phase);
		inline byte phaseDecrement(byte phase);

		modbus& _mb;
		modbusAddr_t _signalAAddr; 
		modbusAddr_t _signalBAddr;
		
		std::atomic<bool> _lostTrackOfEncoder = false;
		std::atomic<int> _rotationCounter = 0; 
		std::atomic<std::chrono::milliseconds> _updateTime = std::chrono::milliseconds(100);
		std::atomic<bool> _treadRun = true;

		modbus _privateModbus;
		std::thread _updateThread;
	};
}