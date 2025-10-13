#pragma once
#include "FactoryIOGeneral.h"
#include <vector>

namespace FactoryIO {
	

	class emmiter_t {
	public:
		emmiter_t(modbus& mb, modbusAddr_t emmitAddr, modbusAddr_t partAddr, modbusAddr_t baseAddr);
		void emmit(bool state);
		void setParts(const std::vector<Parts_t> partsToEmmit);
		void setBase(const std::vector<Bases_t> basesToEmmit);

	private:
		void updateFactoryIO();

		modbusAddr_t _emmitAddr; 
		modbusAddr_t _partAddr;
		modbusAddr_t _baseAddr;

		bool _emmit = false;
		int _part = 0;
		int _base = 0;
		modbus& _mb;
		
	};
}