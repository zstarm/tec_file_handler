#include "tec_variable.h"

namespace tec {
	//--------------------------------------------------------------------------------------------
	// TECPLOT VARIABLE
	//--------------------------------------------------------------------------------------------
	variable::variable() : name("V") {
	}

	variable::variable(std::string vname) : name(vname) {
	}
	
	variable::variable(std::string vname, std::vector<float> &&data, int zidx) : name(vname) {
		if(zidx > 0) {
			add_emptyInstances(zidx);
		}
		if(zidx >= 0) {
			subzoneData.emplace_back(std::move(data));
		}
	}
	
	variable::variable(std::string vname, std::vector<double> &&data, int zidx) : name(vname) {
		if(zidx > 0) {
			add_emptyInstances(zidx);
		}
		if(zidx >= 0) {
			subzoneData.emplace_back(std::move(data));
		}
	}
	
	variable::variable(std::string vname, std::vector<int32_t> &&data, int zidx) : name(vname) {
		if(zidx > 0) {
			add_emptyInstances(zidx);
		}
		if(zidx >= 0) {
			subzoneData.emplace_back(std::move(data));
		}
	}

	variable::variable(std::string vname, std::vector<int16_t> &&data, int zidx) : name(vname) {
		if(zidx > 0) {
			add_emptyInstances(zidx);
		}
		if(zidx >= 0) {
			subzoneData.emplace_back(std::move(data));
		}
	}

	variable::variable(std::string vname, std::vector<uint8_t> &&data, int zidx) : name(vname) {
		if(zidx > 0) {
			add_emptyInstances(zidx);
		}
		if(zidx >= 0) {
			subzoneData.emplace_back(std::move(data));
		}
	}

	/*
	variable::variable(const variable &obj) : name(obj.name), subzoneData(obj.subzoneData) {}

	variable::variable(variable &&obj) : name(std::move(obj.name)), subzoneData(std::move(obj.subzoneData)) {}
	*/

	variable::~variable() {}

	void variable::add_emptyInstances(int numInstances) {
		for(int i = 0; i < numInstances; i++) {
			subzoneData.emplace_back(zoneData(dataTypeFlag::singlePrecision));
		}
	}

	void variable::resize_zone(int zone, int _size, dataTypeFlag T) {
		if(zone >= subzoneData.size()) {
			int N = zone-subzoneData.size();
			if(N > 0) {
				add_emptyInstances(N);
			}
			/*
			while(zone > subzoneData.size()) {
				//push back empty type for zones where the variable is passive/unused 
				subzoneData.emplace_back(dataTypeFlag::singlePrecision);
			}
			*/
			subzoneData.emplace_back(T);
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

	std::string& variable::get_name() {
		return name;
	}

	void variable::add_zoneData(int zidx, std::vector<float> &&data) {
		int N = zidx-subzoneData.size();			
		if(N < 0) {
			//SUBZONE DATA ALREADY EXISTS -> OVERWRITE??
		}
		else {
			if(N > 0) {
				add_emptyInstances(N);
			}
			subzoneData.emplace_back(std::move(data));
		}
	}
	
	void variable::add_zoneData(int zidx, std::vector<double> &&data) {
		int N = zidx-subzoneData.size();			
		if(N < 0) {
			//SUBZONE DATA ALREADY EXISTS -> OVERWRITE??
		}
		else {
			if(N > 0) {
				add_emptyInstances(N);
			}
			subzoneData.emplace_back(std::move(data));
		}
	}

	void variable::add_zoneData(int zidx, std::vector<int32_t> &&data) {
		int N = zidx-subzoneData.size();			
		if(N < 0) {
			//SUBZONE DATA ALREADY EXISTS -> OVERWRITE??
		}
		else {
			if(N > 0) {
				add_emptyInstances(N);
			}
			subzoneData.emplace_back(std::move(data));
		}
	}

	void variable::add_zoneData(int zidx, std::vector<int16_t> &&data) {
		int N = zidx-subzoneData.size();			
		if(N < 0) {
			//SUBZONE DATA ALREADY EXISTS -> OVERWRITE??
		}
		else {
			if(N > 0) {
				add_emptyInstances(N);
			}
			subzoneData.emplace_back(std::move(data));
		}
	}

	void variable::add_zoneData(int zidx, std::vector<uint8_t> &&data) {
		int N = zidx-subzoneData.size();			
		if(N < 0) {
			//SUBZONE DATA ALREADY EXISTS -> OVERWRITE??
		}
		else {
			if(N > 0) {
				add_emptyInstances(N);
			}
			subzoneData.emplace_back(std::move(data));
		}
	}

	zoneData& variable::operator[](int zoneIdx) {
		if(zoneIdx < subzoneData.size()) {
			return subzoneData[zoneIdx];
		}
		throw containerError("zone index exceeds subzone vector limits of " + std::to_string(subzoneData.size()) + "for variable " + name);
	}

}
