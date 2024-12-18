#include "tec_variable.h"

namespace tec {
	//--------------------------------------------------------------------------------------------
	// TECPLOT VARIABLE
	//--------------------------------------------------------------------------------------------
	variable::variable() : name("V") {}

	variable::variable(std::string vname) : name(vname) {}

	/*
	variable::variable(const variable &obj) : name(obj.name), subzoneData(obj.subzoneData) {}

	variable::variable(variable &&obj) : name(std::move(obj.name)), subzoneData(std::move(obj.subzoneData)) {}
	*/
	variable::~variable() {}

	void variable::resize_zone(int zone, int _size, dataTypeFlag T) {
		if(zone >= subzoneData.size()) {
			while(zone > subzoneData.size()) {
				//push back empty type for zones where the variable is passive/unused 
				subzoneData.push_back(dataTypeFlag::singlePrecision);
			}
			subzoneData.push_back(T);
		}
		
		switch(T) {
			case dataTypeFlag::singlePrecision:
				subzoneData[zone].resize<float>(_size, (float)0.0);
				break;
			case dataTypeFlag::doublePrecision:
				subzoneData[zone].resize<double>(_size, (double)0.0);
				break;
			case dataTypeFlag::int32:
				subzoneData[zone].resize<int32_t>(_size, (int32_t)0);
				break;
			case dataTypeFlag::int16:
				subzoneData[zone].resize<int16_t>(_size, (int16_t)0);
				break;
			case dataTypeFlag::byte:
				subzoneData[zone].resize<uint8_t>(_size, (uint8_t)0);
				break;
			default:
				throw containerError("could not resize zone for the variable: " + name);
		}
	}

	void variable::modify_name(std::string vname) {
		name = vname;
	}

	std::string variable::get_name() {
		return name;
	}

	zoneData& variable::operator[](int zoneIdx) {
		if(zoneIdx < subzoneData.size()) {
			return subzoneData[zoneIdx];
		}
		throw containerError("zone index exceeds subzone vector limits of " + std::to_string(subzoneData.size()) + "for variable " + name);
	}

}
