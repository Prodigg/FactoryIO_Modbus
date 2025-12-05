#include "Timers.h"

FactoryIO::primitiveTimer_t::primitiveTimer_t(
	const std::chrono::high_resolution_clock::duration triggerDurration, 
	std::function<void(void)> func) : 
	_triggerTime(triggerDurration), 
	_function(func) { }

void FactoryIO::primitiveTimer_t::setActive(const bool active) {
	_active = active;
}

std::chrono::high_resolution_clock::duration FactoryIO::primitiveTimer_t::getTimePassed() {
	if (!_active)
		return std::chrono::high_resolution_clock::duration();
	return std::chrono::high_resolution_clock::now() - _startTime;
}

void FactoryIO::primitiveTimer_t::update() {
	if (!_previausActive && _active)
		_startTime = std::chrono::high_resolution_clock::now();

	if (!_active) {
		_functionExecuted = false;
		return;
	}
	if (std::chrono::high_resolution_clock::now() - _startTime >= _triggerTime) {
		_function();
		_functionExecuted = true;
	}
}

FactoryIO::TON_t::TON_t(const std::chrono::high_resolution_clock::duration triggerDurration) :
	primitiveTimer_t(
		triggerDurration, 
		[&](void) {_triggered = true; }
	) { }

void FactoryIO::TON_t::setActive(bool active) {
	primitiveTimer_t::setActive(active);
	if (!active)
		_triggered = false;
}