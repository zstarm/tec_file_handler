#include "tec_zoneData.h"

namespace tec {
	//---------------------------------------------------------------------------------------------
	// TECPLOT DATA
	//---------------------------------------------------------------------------------------------
	zoneData::zoneData() : T(dataTypeFlag::singlePrecision) {
	}

	zoneData::zoneData(zoneData &obj) : T(obj.T) {
		switch(T) {
			case dataTypeFlag::singlePrecision:
				if(obj.float_content) {
					float_content = std::make_unique<std::vector<float>>(*obj.float_content);
				}
				break;
			case dataTypeFlag::doublePrecision:
				if(obj.double_content) {
					double_content = std::make_unique<std::vector<double>>(*obj.double_content);
				}
				break;
			case dataTypeFlag::int32:
				if(obj.int32_content) {
					int32_content = std::make_unique<std::vector<int32_t>>(*obj.int32_content);
				}
				break;
			case dataTypeFlag::int16:
				if(obj.int16_content) {
					int16_content = std::make_unique<std::vector<int16_t>>(*obj.int16_content);
				}
				break;
			case dataTypeFlag::byte:
				if(obj.byte_content) {
					byte_content = std::make_unique<std::vector<uint8_t>>(*obj.byte_content);
				}
				break;
			default:
				throw containerError("issue with copying zoneData object");
		}
	}

	zoneData::zoneData(zoneData &&obj) : T(std::move(obj.T)) {
		float_content = std::move(obj.float_content);
		double_content = std::move(obj.double_content);
		int32_content = std::move(obj.int32_content);
		int16_content = std::move(obj.int16_content);
		byte_content = std::move(obj.byte_content);
	}

	zoneData::zoneData(dataTypeFlag type) : T(type) {
	}

	zoneData::zoneData(dataTypeFlag type, int size) : T(type) {
		allocate(size);
	}

	zoneData::zoneData(int size, float val) : T(dataTypeFlag::singlePrecision) {
		allocate<float>(size, val);
	}

	zoneData::zoneData(int size, double val) : T(dataTypeFlag::doublePrecision) {
		allocate<double>(size, val);
	}

	zoneData::zoneData(int size, int32_t val) : T(dataTypeFlag::int32) {
		allocate<int32_t>(size, val);
	}

	zoneData::zoneData(int size, int16_t val) : T(dataTypeFlag::int16) {
		allocate<int16_t>(size, val);
	}

	zoneData::zoneData(int size, uint8_t val) : T(dataTypeFlag::byte) {
		allocate<uint8_t>(size, val);
	}

	zoneData::~zoneData() {}

	void zoneData::allocate(int size) {
		switch(T) {
			case dataTypeFlag::singlePrecision:
				float_content = std::make_unique<std::vector<float>>(size);
				break;
			case dataTypeFlag::doublePrecision:
				double_content = std::make_unique<std::vector<double>>(size);
				break;
			case dataTypeFlag::int32:
				int32_content = std::make_unique<std::vector<int32_t>>(size);
				break;
			case dataTypeFlag::int16:
				int16_content = std::make_unique<std::vector<int16_t>>(size);
				break;
			case dataTypeFlag::byte:
				byte_content = std::make_unique<std::vector<uint8_t>>(size);
				break;
			default:
				std::cout << "ERROR!: unrecognized data type for zoneData, no memory allcoation made" << std::endl;
		}
	}

	template <typename DT>
	void zoneData::allocate(int size, DT val) {
		switch(T) {
			case dataTypeFlag::singlePrecision:
				float_content = std::make_unique<std::vector<float>>(size, val);
				break;
			case dataTypeFlag::doublePrecision:
				double_content = std::make_unique<std::vector<double>>(size, val);
				break;
			case dataTypeFlag::int32:
				int32_content = std::make_unique<std::vector<int32_t>>(size, val);
				break;
			case dataTypeFlag::int16:
				int16_content = std::make_unique<std::vector<int16_t>>(size, val);
				break;
			case dataTypeFlag::byte:
				byte_content = std::make_unique<std::vector<uint8_t>>(size, val);
				break;
			default:
				std::cout << "ERROR!: unrecognized data type for zoneData, no memory allcoation made" << std::endl;
		}
	}
		
	void zoneData::resize(int new_size) {
		switch(T) {
			case dataTypeFlag::singlePrecision:
				if(float_content) {
					float_content->resize(new_size);
				}
				else {
					allocate(new_size);
				}
				break;
			case dataTypeFlag::doublePrecision:
				if(double_content) {
					double_content->resize(new_size);
				}
				break;
			case dataTypeFlag::int32:
				if(int32_content) {
					int32_content->resize(new_size);
				}
				else {
					allocate(new_size);
				}
				break;
			case dataTypeFlag::int16:
				if(int16_content) {
					int16_content->resize(new_size);
				}
				else {
					allocate(new_size);
				}
				break;
			case dataTypeFlag::byte:
				if(byte_content) {
					byte_content->resize(new_size);
				}
				else {
					allocate(new_size);
				}
				break;
			default:
				std::cout << "ERROR!: unrecognized data type for zoneData, no resizing was performed" << std::endl;
		}
	}

	void zoneData::push_back(float val) {
		if(float_content) {
			float_content->push_back(val);
		}
		else {
			if(T != dataTypeFlag::singlePrecision) {
				std::cout << "ERROR!: tried to push_back incorrect type to zoneData, no data inserted" << std::endl;
			}
			else {
				allocate<float>(1, val);
			}
		}
	}

	void zoneData::push_back(double val) {
		if(double_content) {
			double_content->push_back(val);
		}
		else {
			if(T != dataTypeFlag::doublePrecision) {
				std::cout << "ERROR!: tried to push_back incorrect type to zoneData, no data inserted" << std::endl;
			}
			else {
				allocate<double>(1, val);
			}
		}
	}

	void zoneData::push_back(int32_t val) {
		if(int32_content) {
			int32_content->push_back(val);
		}
		else {
			if(T != dataTypeFlag::int32) {
				std::cout << "ERROR!: tried to push_back incorrect type to zoneData, no data inserted" << std::endl;
			}
			else {
				allocate<int32_t>(1, val);
			}
		}
	}

	void zoneData::push_back(int16_t val) {
		if(int16_content) {
			int16_content->push_back(val);
		}
		else {
			if(T != dataTypeFlag::int16) {
				std::cout << "ERROR!: tried to push_back incorrect type to zoneData, no data inserted" << std::endl;
			}
			else {
				allocate<int16_t>(1, val);
			}
		}
	}

	void zoneData::push_back(uint8_t val) {
		if(byte_content) {
			byte_content->push_back(val);
		}
		else {
			if(T != dataTypeFlag::byte) {
				std::cout << "ERROR!: tried to push_back incorrect type to zoneData, no data inserted" << std::endl;
			}
			else {
				allocate<uint8_t>(1, val);
			}
		}
	}

	dataTypeFlag zoneData::type() {
		return T;
	}

	int zoneData::get_array_size() {
		switch(T) {
			case dataTypeFlag::singlePrecision:
				if(float_content) {
					return float_content->size();
				}
				else {
					return 0;	
				}
				break;
			case dataTypeFlag::doublePrecision:
				if(double_content) {
					return double_content->size();
				}
				else {
					return 0;
				}
				break;
			case dataTypeFlag::int32:
				if(int32_content) {
					return int32_content->size();
				}
				else {
					return 0;
				}
				break;
			case dataTypeFlag::int16:
				if(int16_content) {
					return int16_content->size();
				}
				else {
					return 0;
				}
				break;
			case dataTypeFlag::byte:
				if(byte_content) {
					return byte_content->size();
				}
				else {
					return 0;
				}
				break;
			default:
				return 0;
		}

		return 0;
	}
	
	float& zoneData::get_float(int idx) {
		if(float_content) {
			if(idx < (*float_content).size()) {
				return (*float_content)[idx];
			}

			throw containerError("index is out of bounds when trying to retrieve float data");
		}

		throw containerError("no float data is allocated when trying to retrieve float data");
	}

	double& zoneData::get_double(int idx) {
		if(double_content) {
			if(idx < (*double_content).size()) {
				return (*double_content)[idx];
			}

			throw containerError("index is out of bounds when trying to retrieve double data");
		}

		throw containerError("no double data is allocated when trying to retrieve double data");
	}

	int32_t& zoneData::get_int32(int idx) {
		if(int32_content) {
			if(idx < (*int32_content).size()) {
				return (*int32_content)[idx];
			}

			throw containerError("index is out of bounds when trying to retrieve int32 data");
		}

		throw containerError("no int32_t data is allocated when trying to retrieve int32 data");
	}

	int16_t& zoneData::get_int16(int idx) {
		if(int16_content) {
			if(idx < (*int16_content).size()) {
				return (*int16_content)[idx];
			}

			throw containerError("index is out of bounds when trying to retrieve int16 data");
		}

		throw containerError("no int16_t data is allocated when trying to retrieve int16 data");
	}

	uint8_t& zoneData::get_byte(int idx) {
		if(byte_content) {
			if(idx < (*byte_content).size()) {
				return (*byte_content)[idx];
			}

			throw containerError("index is out of bounds when trying to retrieve byte data");
		}

		throw containerError("no byte data is allocated when trying to retrieve byte data");
	}

}
