#include "pch.h"
#include "CppUnitTest.h"
#include <FactoryIO.h>
#include <chrono>
#include <thread>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FactoryIO::internal::testing;

namespace _1FactoryIOLibTest_module {
	TEST_CLASS(convayorGate) {
		TEST_METHOD(exceptions) {
			modbus mb = modbus("127.0.0.1", 502);
			
			Assert::ExpectException<std::runtime_error, std::function<void(void)>>(
				[&](void) -> void {
					FactoryIO::convayorGate_t gate(mb, 0, 0, 0, 0, FactoryIO::ConvayorMode_t::DIGITAL, 0, FactoryIO::NO_MODBUS_ADDR);
					gate.isGateOpend();
				}, L"No exception when isGateOpen is called and no Modbus Address was provided for opend"
			);

			mb.modbus_close();
		}
	};
}