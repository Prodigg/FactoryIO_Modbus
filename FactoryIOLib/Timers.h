#pragma once
#include <chrono>
#include <functional>

namespace FactoryIO {
	class primitiveTimer_t {
	public:
		primitiveTimer_t(const std::chrono::high_resolution_clock::duration triggerDurration, std::function<void(void)> func);
		virtual void setActive(const bool active);
		inline bool getActive() const { return _active; }

		inline void setTriggerTime(const std::chrono::high_resolution_clock::duration triggerDurration) { _triggerTime = triggerDurration; }
		std::chrono::high_resolution_clock::duration getTimePassed();

		inline void setTriggerFuncion(std::function<void(void)> func) { _function = func; }

		virtual void update();
	private:
		std::chrono::high_resolution_clock::duration _triggerTime;
		std::chrono::high_resolution_clock::time_point _startTime;

		std::function<void(void)> _function;
		bool _functionExecuted = false;
		bool _active = false;
		bool _previausActive = false;
	};

	class TON_t : private primitiveTimer_t {
	public:
		TON_t(const std::chrono::high_resolution_clock::duration triggerDurration);
		using primitiveTimer_t::setTriggerTime;
		using primitiveTimer_t::getActive;
		using primitiveTimer_t::getTimePassed;
		void setActive(bool active);
		virtual void update() override;
		virtual inline bool hasTriggered() const { return _triggered; }
	private:
		bool _triggered = false;
	};

	class TOF_t : public TON_t {
	public:
		inline bool hasTriggered() const override { return !TON_t::hasTriggered(); }
	};
}