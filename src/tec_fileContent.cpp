#include "tec_fileContent.h"

//-----------------------------------------------------------------------------------------------
// TECPLOT ZONE DETAILS
//-----------------------------------------------------------------------------------------------
tec_zoneDetails::tec_zoneDetails(int zid, size_t vars) : nVars(vars), zoneID(zid), zoneTitle("ZONE 001"), hasSharedVars(false), hasPassiveVars(false) {
	zoneType = zoneTypeFlag::ordered; 
	dataPacking = formattingFlag::point;
	I = 0;
	J = 0;
	K = 0;
	strandID = 0;
	solutionTime = 0.0;

	zone_sharedVars.resize(nVars);
	zone_passiveVars.resize(nVars);
	zone_varDTs.resize(nVars);

}

tec_zoneDetails::~tec_zoneDetails() {
}

tec_zoneDetails::tec_zoneDetails(tec_zoneDetails &obj) : zoneID(obj.zoneID) {
	
	nVars = obj.nVars;

	I = obj.I;
	J = obj.J;
	K = obj.K;

	dataPacking = obj.dataPacking;
	zoneType = obj.zoneType;
	zoneTitle = obj.zoneTitle;

	strandID = obj.strandID;
	solutionTime = obj.solutionTime;

	hasSharedVars = obj.hasSharedVars;
	hasPassiveVars = obj.hasPassiveVars;

	zone_varDTs = obj.zone_varDTs;
	zone_sharedVars = obj.zone_sharedVars;
	zone_passiveVars = obj.zone_passiveVars;

}

tec_zoneDetails::tec_zoneDetails(tec_zoneDetails &&obj) : zoneID(std::move(obj.zoneID)) {
	
	nVars = std::move(obj.nVars);

	I = std::move(obj.I);
	J = std::move(obj.J);
	K = std::move(obj.K);

	dataPacking = std::move(obj.dataPacking);
	zoneType = std::move(obj.zoneType);
	zoneTitle = std::move(obj.zoneTitle);

	strandID = std::move(obj.strandID);
	solutionTime = std::move(obj.solutionTime);
	
	hasSharedVars = std::move(obj.hasSharedVars);
	hasPassiveVars = std::move(obj.hasPassiveVars);

	zone_varDTs = std::move(obj.zone_varDTs);
	zone_sharedVars = std::move(obj.zone_sharedVars);
	zone_passiveVars = std::move(obj.zone_passiveVars);
}

void tec_zoneDetails::set_IJKSize(char IJK, int size) {
	switch(IJK) {
		case 'I':
			I = size;
			break;
		
		case 'J':
			J = size;
			break;

		case 'K':
			K = size;
			break;

		default:
			std::cout << "ERROR!: unrecognized character identifer for setting IJKSize, use \"I\", \"J\", or \"K\" " << std::endl;
	}
}

void tec_zoneDetails::set_formatType(char formattingType) {
	if(formattingType == 'P' || formattingType == 'p') {
		dataPacking = formattingFlag::point;
	}
	else if(formattingType == 'B' || formattingType == 'b') {
		dataPacking = formattingFlag::block;
	}
	else {
		std::cout << "ERROR!: Unrecognized formatting type for set_formatType, current formatting is set to ";
		if(dataPacking == formattingFlag::point) {
			std::cout << "\"POINT\"" << std::endl;
		}
		else {
			std::cout << "\"BLOCK\"" << std::endl;
		}
	}
}

void tec_zoneDetails::set_zoneType(char type) {
	if(type == 'O' || type == 'o') {
		zoneType = zoneTypeFlag::ordered;
	}

	else if(type == 'F' || type == 'f') {
		zoneType = zoneTypeFlag::FE;
	}
	else {
		std::cout << "ERROR!: Unrecognized zone type for set_zoneType, current zone type is set to ";
		if(zoneType == zoneTypeFlag::ordered) {
			std::cout << "\"ORDERED\"" << std::endl;
		}
		else {
			std::cout << "\"FINITE ELEMENT\"" << std::endl;
		}
	}
}

void tec_zoneDetails::set_zoneTitle(std::string title) {
	zoneTitle = title;
}

void tec_zoneDetails::set_strandID(int strand) {
	strandID = strand;
}

void tec_zoneDetails::set_solutionTime(double time) {
	solutionTime = time;
}

void tec_zoneDetails::set_varDT(int vidx, dataTypeFlag type, bool resize) {
	if(vidx < nVars) {
		zone_varDTs[vidx] = type;
	}
	else {
		if(resize) {
			zone_varDTs.resize(++vidx, type);
			nVars = vidx;
		}
		else {
			throw tec_containerError("variable index is out of range for setting data type!");
		}
	}
}

void tec_zoneDetails::set_sharedVar(int vidx, sharedVarFlag flag, bool resize) {
	if(vidx < nVars) {
		zone_sharedVars[vidx] = flag;
		if(flag == sharedVarFlag::shared && !hasSharedVars) {
			hasPassiveVars = true;	
		}
		else if(flag == sharedVarFlag::nonshared && hasSharedVars) {
			//check to see if other passive vars exist before changing flag
			for(int v = 0; v < nVars; v++) {
				if(zone_sharedVars[v] == sharedVarFlag::shared) {
					break;
				}
				else if(v+1 == nVars) {
					hasSharedVars = false;
				}
			}
		}
	}
	else {
		if(resize) {
			zone_sharedVars.resize(++vidx, flag);
			nVars = vidx;
			if(flag != (sharedVarFlag)hasSharedVars) {
				hasSharedVars = (bool)flag;
			}
		}
		else {
			throw tec_containerError("variable index is out of range for setting shared flag!");
		}
	}
}

void tec_zoneDetails::set_passiveVar(int vidx, passiveVarFlag flag, bool resize) {
	if(vidx < nVars) {
		zone_passiveVars[vidx] = flag;
		if(flag == passiveVarFlag::passive && !hasPassiveVars) {
			hasPassiveVars = true;	
		}
		else if(flag == passiveVarFlag::nonpassive && hasPassiveVars) {
			//check to see if other passive vars exist before changing flag
			for(int v = 0; v < nVars; v++) {
				if(zone_passiveVars[v] == passiveVarFlag::passive) {
					break;
				}
				else if(v+1 == nVars) {
					hasPassiveVars = false;
				}
			}
		}
	}
	else {
		if(resize) {
			zone_passiveVars.resize(++vidx, flag);
			nVars = vidx;
			if(flag != (passiveVarFlag)hasPassiveVars) {
				hasPassiveVars = (bool)flag;
			}
		}
		else {
			throw tec_containerError("variable index is out of range for setting passive flag!");
		}
	}
}

formattingFlag tec_zoneDetails::get_formattingType() {
	return dataPacking;
}

zoneTypeFlag tec_zoneDetails::get_zoneType() {
	return zoneType;
}

int tec_zoneDetails::get_size() {
	if(I || J || K) {
		int s = 1;

		if(I) {
			s *= I;
		}
		if(J) {
			s *= J;
		}
		if(K) {
			s *= K;
		}

		return s;
	}
	return 0;
}

int tec_zoneDetails::get_Imax() {
	return I;
}

int tec_zoneDetails::get_Jmax() {
	return J;
}

int tec_zoneDetails::get_Kmax() {
	return K;
}

std::vector<dataTypeFlag>* tec_zoneDetails::get_varDTs() {
	if(!zone_varDTs.size()) {
		throw tec_containerError("variable type vector is empty!");
	}
	return &zone_varDTs;
}

//-----------------------------------------------------------------------------------------------
// TECPLOT DATA
//-----------------------------------------------------------------------------------------------
tec_data::tec_data() : T(dataTypeFlag::singlePrecision) {
}

tec_data::tec_data(tec_data &obj) : T(obj.T) {
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
			throw tec_containerError("issue with copying tec_data object");
	}
}

tec_data::tec_data(tec_data &&obj) : T(std::move(obj.T)) {
	float_content = std::move(obj.float_content);
	double_content = std::move(obj.double_content);
	int32_content = std::move(obj.int32_content);
	int16_content = std::move(obj.int16_content);
	byte_content = std::move(obj.byte_content);
}

tec_data::tec_data(dataTypeFlag type) : T(type) {
}

tec_data::tec_data(dataTypeFlag type, int size) : T(type) {
	allocate(size);
}

tec_data::tec_data(int size, float val) : T(dataTypeFlag::singlePrecision) {
	allocate<float>(size, val);
}

tec_data::tec_data(int size, double val) : T(dataTypeFlag::doublePrecision) {
	allocate<double>(size, val);
}

tec_data::tec_data(int size, int32_t val) : T(dataTypeFlag::int32) {
	allocate<int32_t>(size, val);
}

tec_data::tec_data(int size, int16_t val) : T(dataTypeFlag::int16) {
	allocate<int16_t>(size, val);
}

tec_data::tec_data(int size, uint8_t val) : T(dataTypeFlag::byte) {
	allocate<uint8_t>(size, val);
}

tec_data::~tec_data() {}

void tec_data::allocate(int size) {
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
			std::cout << "ERROR!: unrecognized data type for tec_data, no memory allcoation made" << std::endl;
	}
}

template <typename DT>
void tec_data::allocate(int size, DT val) {
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
			std::cout << "ERROR!: unrecognized data type for tec_data, no memory allcoation made" << std::endl;
	}
}
	
template<typename DT> 
void tec_data::resize(int new_size, DT val) {
	
	switch(T) {
		case dataTypeFlag::singlePrecision:
			if(float_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				float_content.reset();
			}
			break;
		case dataTypeFlag::doublePrecision:
			if(double_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				double_content.reset();
			}
			break;
		case dataTypeFlag::int32:
			if(int32_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				int32_content.reset();
			}
			break;
		case dataTypeFlag::int16:
			if(int16_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				int16_content.reset();
			}
			break;
		case dataTypeFlag::byte:
			if(byte_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				byte_content.reset();
			}
			break;
		default:
			std::cout << "ERROR!: unrecognized data type for tec_data, defaulting to \"float\" data type" << std::endl;
	}

	uint8_t tmpb;
	int16_t tmpis;
	int32_t tmpil;
	double tmpd;
	if(std::is_same<DT, uint8_t>::value) {
		T = dataTypeFlag::byte;
	}
	else if(std::is_same<DT, int16_t>::value) {
		T = dataTypeFlag::int16;
	}
	else if(std::is_same<DT,int32_t>::value) {
		T = dataTypeFlag::int32;
	}
	else if(std::is_same<DT,double>::value) {
		T = dataTypeFlag::doublePrecision;
	}
	else {
		T = dataTypeFlag::singlePrecision;
	}
	allocate<DT>(new_size, val); 
}

void tec_data::resize(int new_size) {
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
			std::cout << "ERROR!: unrecognized data type for tec_data, no resizing was performed" << std::endl;
	}
}

void tec_data::push_back(float val) {
	if(float_content) {
		float_content->push_back(val);
	}
	else {
		if(T != dataTypeFlag::singlePrecision) {
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data, no data inserted" << std::endl;
		}
		else {
			allocate<float>(1, val);
		}
	}
}

void tec_data::push_back(double val) {
	if(double_content) {
		double_content->push_back(val);
	}
	else {
		if(T != dataTypeFlag::doublePrecision) {
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data, no data inserted" << std::endl;
		}
		else {
			allocate<double>(1, val);
		}
	}
}

void tec_data::push_back(int32_t val) {
	if(int32_content) {
		int32_content->push_back(val);
	}
	else {
		if(T != dataTypeFlag::int32) {
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data, no data inserted" << std::endl;
		}
		else {
			allocate<int32_t>(1, val);
		}
	}
}

void tec_data::push_back(int16_t val) {
	if(int16_content) {
		int16_content->push_back(val);
	}
	else {
		if(T != dataTypeFlag::int16) {
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data, no data inserted" << std::endl;
		}
		else {
			allocate<int16_t>(1, val);
		}
	}
}

void tec_data::push_back(uint8_t val) {
	if(byte_content) {
		byte_content->push_back(val);
	}
	else {
		if(T != dataTypeFlag::byte) {
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data, no data inserted" << std::endl;
		}
		else {
			allocate<uint8_t>(1, val);
		}
	}
}

dataTypeFlag tec_data::type() {
	return T;
}

int tec_data::get_array_size() {
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
float& tec_data::get_float(int idx) {
	if(float_content) {
		if(idx < (*float_content).size()) {
			return (*float_content)[idx];
		}

		throw tec_containerError("index is out of bounds when trying to retrieve float data");
	}

	throw tec_containerError("no float data is allocated when trying to retrieve float data");
}

double& tec_data::get_double(int idx) {
	if(double_content) {
		if(idx < (*double_content).size()) {
			return (*double_content)[idx];
		}

		throw tec_containerError("index is out of bounds when trying to retrieve double data");
	}

	throw tec_containerError("no double data is allocated when trying to retrieve double data");
}

int32_t& tec_data::get_int32(int idx) {
	if(int32_content) {
		if(idx < (*int32_content).size()) {
			return (*int32_content)[idx];
		}

		throw tec_containerError("index is out of bounds when trying to retrieve int32 data");
	}

	throw tec_containerError("no int32_t data is allocated when trying to retrieve int32 data");
}

int16_t& tec_data::get_int16(int idx) {
	if(int16_content) {
		if(idx < (*int16_content).size()) {
			return (*int16_content)[idx];
		}

		throw tec_containerError("index is out of bounds when trying to retrieve int16 data");
	}

	throw tec_containerError("no int16_t data is allocated when trying to retrieve int16 data");
}

uint8_t& tec_data::get_byte(int idx) {
	if(byte_content) {
		if(idx < (*byte_content).size()) {
			return (*byte_content)[idx];
		}

		throw tec_containerError("index is out of bounds when trying to retrieve byte data");
	}

	throw tec_containerError("no byte data is allocated when trying to retrieve byte data");
}


//-----------------------------------------------------------------------------------------------
// TECPLOT VARIABLE
//-----------------------------------------------------------------------------------------------
tec_variable::tec_variable() : name("V") {}

tec_variable::tec_variable(std::string vname) : name(vname) {}

tec_variable::tec_variable(tec_variable &obj) : subzoneData(obj.subzoneData) {
	name = obj.name;
}

tec_variable::tec_variable(tec_variable &&obj) : subzoneData(std::move(obj.subzoneData)){
	name = std::move(obj.name);
}

tec_variable::~tec_variable() {
}

void tec_variable::resize_zone(int zone, int _size, dataTypeFlag T) {
	while(zone >= subzoneData.size()) {
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
			throw tec_containerError("could not resize zone for the variable: " + name);
	}
}

void tec_variable::modify_name(std::string vname) {
	name = vname;
}

tec_data& tec_variable::operator[](int zoneIdx) {
	if(zoneIdx < subzoneData.size()) {
		return subzoneData[zoneIdx];
	}
	throw tec_containerError("zone index exceeds subzone vector limits");
}


//-----------------------------------------------------------------------------------------------
// TECPLOT FILE CONTENT
//-----------------------------------------------------------------------------------------------
tec_fileContent::tec_fileContent() : fType(fileTypeFlag::full), title("N/A") {}

tec_fileContent::~tec_fileContent() {}

void tec_fileContent::print_headerDetails() {
	std::cout << "TECPLOT FILE HEADER DETAILS" << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	//title
	std::cout << "TITLE: " << title << std::endl;

	//file type
	std::cout << "FILE TYPE: "; 
	switch(fType) {
		case fileTypeFlag::full:
			std::cout << "FULL" << std::endl;
			break;
		case fileTypeFlag::grid:
			std::cout << "GRID" << std::endl;
			break;
		case fileTypeFlag::solution:
			std::cout << "SOLUTION" << std::endl;
			break;
	}

	//variable list
	std::cout << "VARIABLES: ";
	for(int v = 0; v < variables.size(); v++) {
		std::cout << variables[v].name;
		//ternary operator to determine if at the end of the variable list or not
		v != variables.size()-1 ? std::cout << ", " : std::cout << std::endl;
	}

}

void tec_fileContent::print_zoneDetails(int zidx) {
	std::cout << "TECPLOT ZONE #" << zoneDetails[zidx].zoneID << " DETAILS" << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	
	//zone title
	std::cout << "ZONE TITLE: " << zoneDetails[zidx].zoneTitle << std::endl;

	//zone type
	std::cout << "ZONE TYPE: ";
	zoneDetails[zidx].zoneType == zoneTypeFlag::ordered ? std::cout << "ORDERED" : std::cout << "FINITE ELEMENT";
	std::cout << std::endl;

	//zone formatting/datapacking 
	std::cout << "ZONE FORMATTING: ";
	zoneDetails[zidx].dataPacking == formattingFlag::point ? std::cout << "POINT" : std::cout << "BLOCK";
	std::cout << std::endl;

	std::cout << "ZONE DIMENSIONS: I = " << zoneDetails[zidx].I << ", J = " << zoneDetails[zidx].J << ", K = " << zoneDetails[zidx].K << std::endl;

	//transient datasets only
	if(zoneDetails[zidx].strandID) {
		//strand ID
		std::cout << "ZONE STRAND ID: " << zoneDetails[zidx].strandID << std::endl;
		std::cout << "ZONE SOLUTION TIME: " << zoneDetails[zidx].solutionTime << std::endl;
	}

}

void tec_fileContent::print_zoneData(int zidx) {
	std::cout << "TECPLOT ZONE #" << zoneDetails[zidx].zoneID << " DATA" << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	for(int v = 0; v < variables.size(); v++) {
		std::cout << variables[v].name;
		std::cout << "(";
		switch(variables[v][zidx].T) {
			case dataTypeFlag::singlePrecision:
				std::cout << "SINGLE";
				break;
			case dataTypeFlag::doublePrecision:
				std::cout << "DOUBLE";
				break;
			case dataTypeFlag::int32:
				std::cout << "LONGINT";
				break;
			case dataTypeFlag::int16:
				std::cout << "SHORTINT";
				break;
			case dataTypeFlag::byte:
				std::cout << "BYTE";
				break;
			default:
				throw tec_containerError("issue getting data type for variable: " + variables[v].name + " at zone" + std::to_string(zidx) + " to display");
		}
		std::cout << ")";
		//ternary operator to determine if at the end of the variable list or not
		v != variables.size()-1 ? std::cout << ", " : std::cout << std::endl;
	}
	std::cout << "-----------------------------------------" << std::endl;
	for(int i = 0; i < zoneDetails[zidx].get_size(); i++) {
		for(int v = 0; v < variables.size(); v++) {
			switch(variables[v][zidx].T) {
				case dataTypeFlag::singlePrecision:
					std::cout << variables[v][zidx].get_float(i);
					break;
				case dataTypeFlag::doublePrecision:
					std::cout << variables[v][zidx].get_double(i);
					break;
				case dataTypeFlag::int32:
					std::cout << variables[v][zidx].get_int32(i);
					break;
				case dataTypeFlag::int16:
					std::cout << variables[v][zidx].get_int16(i);
					break;
				case dataTypeFlag::byte:
					std::cout << variables[v][zidx].get_byte(i);
					break;
				default:
					throw tec_containerError("issue getting data type for variable: " + variables[v].name + " at zone" + std::to_string(zidx) + " to display");
			}
			//ternary operator to determine if at the end of the variable list or not
			v != variables.size()-1 ? std::cout << "\t" : std::cout << std::endl;
		}
	}
}

void tec_fileContent::print_fileDetails(bool include_data) {
	print_headerDetails();
	std::cout << std::endl;	
	for(int z = 0; z < zoneDetails.size(); z++) {
		print_zoneDetails(z);
		std::cout << std::endl;	
		if(include_data) {
			print_zoneData(z);
			std::cout << std::endl;	
		}
	}

}
