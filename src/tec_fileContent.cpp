#include "tec_fileContent.h"

tec_fileContent::tec_fileContent() {}

tec_fileContent::~tec_fileContent() {}

tec_data::tec_data() {
}

tec_data::tec_data(tec_data& obj) : T(obj.T) {
	switch((char)T) {
		case (char)dataTypeFlag::singlePrecision:
			if(obj.float_content) {
				float_content = std::make_unique<std::vector<float>>(*obj.float_content);
			}
			break;
		case (char)dataTypeFlag::doublePrecision:
			if(obj.double_content) {
				double_content = std::make_unique<std::vector<double>>(*obj.double_content);
			}
			break;
		case (char)dataTypeFlag::int32:
			if(obj.int32_content) {
				int32_content = std::make_unique<std::vector<int32_t>>(*obj.int32_content);
			}
			break;
		case (char)dataTypeFlag::int16:
			if(obj.int16_content) {
				int16_content = std::make_unique<std::vector<int16_t>>(*obj.int16_content);
			}
			break;
		case (char)dataTypeFlag::byte:
			if(obj.byte_content) {
				byte_content = std::make_unique<std::vector<uint8_t>>(*obj.byte_content);
			}
			break;
		//default:
			//std::cout << "ERROR!: undefined behavior when allocating tec_data" << std::endl;
	}
}

tec_data::tec_data(tec_data&& obj) : T(std::move(obj.T)) {
	float_content = std::move(obj.float_content);
	double_content = std::move(obj.double_content);
	int32_content = std::move(obj.int32_content);
	int16_content = std::move(obj.int16_content);
	byte_content = std::move(obj.byte_content);
}

tec_data::tec_data(dataTypeFlag type) : T(type) {}

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

void tec_data::allocate(int size) {
	switch((char)T) {
		case (char)dataTypeFlag::singlePrecision:
			float_content = std::make_unique<std::vector<float>>(size);
			break;
		case (char)dataTypeFlag::doublePrecision:
			double_content = std::make_unique<std::vector<double>>(size);
			break;
		case (char)dataTypeFlag::int32:
			int32_content = std::make_unique<std::vector<int32_t>>(size);
			break;
		case (char)dataTypeFlag::int16:
			int16_content = std::make_unique<std::vector<int16_t>>(size);
			break;
		case (char)dataTypeFlag::byte:
			byte_content = std::make_unique<std::vector<uint8_t>>(size);
			break;
		default:
			std::cout << "ERROR!: undefined behavior when allocating tec_data" << std::endl;
	}
}

template <typename DT>
void tec_data::allocate(int size, DT val) {
	switch((char)T) {
		case (char)dataTypeFlag::singlePrecision:
			float_content = std::make_unique<std::vector<float>>(size, val);
			break;
		case (char)dataTypeFlag::doublePrecision:
			double_content = std::make_unique<std::vector<double>>(size, val);
			break;
		case (char)dataTypeFlag::int32:
			int32_content = std::make_unique<std::vector<int32_t>>(size, val);
			break;
		case (char)dataTypeFlag::int16:
			int16_content = std::make_unique<std::vector<int16_t>>(size, val);
			break;
		case (char)dataTypeFlag::byte:
			byte_content = std::make_unique<std::vector<uint8_t>>(size, val);
			break;
		default:
			std::cout << "ERROR!: undefined behavior when allocating tec_data" << std::endl;
	}
}
	
template<typename DT> 
void tec_data::resize(int new_size, DT val) {
	
	switch((char)T) {
		case (char)dataTypeFlag::singlePrecision:
			if(float_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				float_content.reset();
			}
			break;
		case (char)dataTypeFlag::doublePrecision:
			if(double_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				double_content.reset();
			}
			break;
		case (char)dataTypeFlag::int32:
			if(int32_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				int32_content.reset();
			}
			break;
		case (char)dataTypeFlag::int16:
			if(int16_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				int16_content.reset();
			}
			break;
		case (char)dataTypeFlag::byte:
			if(byte_content) {
				std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
				byte_content.reset();
			}
			break;
		default:
			std::cout << "ERROR!: undefined behavior when resizing tec_data" << std::endl;
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
	switch((char)T) {
		case (char)dataTypeFlag::singlePrecision:
			if(float_content) {
				float_content->resize(new_size);
			}
			else {
				allocate(new_size);
			}
			break;
		case (char)dataTypeFlag::doublePrecision:
			if(double_content) {
				double_content->resize(new_size);
			}
			break;
		case (char)dataTypeFlag::int32:
			if(int32_content) {
				int32_content->resize(new_size);
			}
			else {
				allocate(new_size);
			}
			break;
		case (char)dataTypeFlag::int16:
			if(int16_content) {
				int16_content->resize(new_size);
			}
			else {
				allocate(new_size);
			}
			break;
		case (char)dataTypeFlag::byte:
			if(byte_content) {
				byte_content->resize(new_size);
			}
			else {
				allocate(new_size);
			}
			break;
		default:
			std::cout << "ERROR!: undefined behavior when resizing tec_data" << std::endl;
	}
}

void tec_data::push_back(float val) {
	if(float_content) {
		float_content->push_back(val);
	}
	else {
		if(T != dataTypeFlag::singlePrecision) {
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data" << std::endl;
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
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data" << std::endl;
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
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data" << std::endl;
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
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data" << std::endl;
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
			std::cout << "ERROR!: tried to push_back incorrect type to tec_data" << std::endl;
		}
		else {
			allocate<uint8_t>(1, val);
		}
	}
}

float& tec_data::fget(int idx) {
	if(float_content) {
		return (*float_content)[idx];
	}

	throw std::logic_error("ERROR!: Either incorrect function called or not data is allocated");
}

double& tec_data::dget(int idx) {
	if(double_content) {
		return (*double_content)[idx];
	}

	throw std::logic_error("ERROR!: Either incorrect function called or not data is allocated");
}

int32_t& tec_data::ilget(int idx) {
	if(float_content) {
		return (*int32_content)[idx];
	}

	throw std::logic_error("ERROR!: Either incorrect function called or not data is allocated");
}

int16_t& tec_data::isget(int idx) {
	if(int16_content) {
		return (*int16_content)[idx];
	}

	throw std::logic_error("ERROR!: Either incorrect function called or not data is allocated");
}

uint8_t& tec_data::bget(int idx) {
	if(byte_content) {
		return (*byte_content)[idx];
	}

	throw std::logic_error("ERROR!: Either incorrect function called or not data is allocated");
}

/*
template <typename DT> DT& tec_data::get(int idx) {
	switch((char)T) {
		case (char)dataTypeFlag::singlePrecision:
			{
				float tmp;	
				if(float_content && (typeid(tmp) == typeid(DT))) {
					return (&DT)(*float_content)[idx];
				}
				break;
			}
		case (char)dataTypeFlag::doublePrecision:
			{
				double tmp;	
				if(double_content && (typeid(tmp) == typeid(DT))) {
					return (&DT)(*double_content)[idx];
				}
				break;
			}
		case (char)dataTypeFlag::int32:
			{
				int32_t tmp;	
				if(int32_content && (typeid(tmp) == typeid(DT))) {
					return (&DT)(*int32_content)[idx];
				}
				break;
			}
		case (char)dataTypeFlag::int16:
			{
				int16_t tmp;	
				if(int16_content && (typeid(tmp) == typeid(DT))) {
					return (&DT)(*int16_content)[idx];
				}
				break;
			}
		case (char)dataTypeFlag::byte:
			{
				uint8_t tmp;	
				if(byte_content && (typeid(tmp) == typeid(DT))) {
					return (&DT)(*byte_content)[idx];
				}
				break;
			}
		default:
			std::cout << "ERROR!: undefined data type" << std::endl;
	}
	throw std::invalid_argument("Could not retrieve reference");
}

//explicit instantations that will be used
template float& tec_data::get(int idx);
template double& tec_data::get(int idx);
template int32_t& tec_data::get(int idx);
template int16_t& tec_data::get(int idx);
template uint8_t& tec_data::get(int idx);

template<typename DT>
DT& tec_data::get(int&& idx) {
	switch((char)T) {
		case (char)dataTypeFlag::singlePrecision:
			{
				float tmp;	
				if(float_content && (typeid(tmp) == typeid(DT))) {
					return &(*float_content)[idx];
				}
				break;
			}
		case (char)dataTypeFlag::doublePrecision:
			{
				double tmp;	
				if(double_content && (typeid(tmp) == typeid(DT))) {
					return &(*double_content)[idx];
				}
				break;
			}
		case (char)dataTypeFlag::int32:
			{
				int32_t tmp;	
				if(int32_content && (typeid(tmp) == typeid(DT))) {
					return &(*int32_content)[idx];
				}
				break;
			}
		case (char)dataTypeFlag::int16:
			{
				int16_t tmp;	
				if(int16_content && (typeid(tmp) == typeid(DT))) {
					return &(*int16_content)[idx];
				}
				break;
			}
		case (char)dataTypeFlag::byte:
			{
				uint8_t tmp;	
				if(byte_content && (typeid(tmp) == typeid(DT))) {
					return &(*byte_content)[idx];
				}
				break;
			}
		default:
			std::cout << "ERROR!: undefined data type" << std::endl;
	}
	throw std::invalid_argument("Could not retrieve reference");
}

*/
tec_data::~tec_data() {}

tec_variable::tec_variable() {}

tec_variable::tec_variable(std::string vname) : name(vname) {}

tec_variable::tec_variable(tec_variable & obj) : subzoneData(obj.subzoneData) {
	name = obj.name;
}

tec_variable::tec_variable(tec_variable && obj) : subzoneData(std::move(obj.subzoneData)){
	name = std::move(obj.name);
}

tec_variable::~tec_variable() {}

void tec_variable::resize_zone(int zone, int _size, dataTypeFlag T) {
	if(zone >= subzoneData.size()) {
		/*
		switch((char)T) {
			case (char)dataTypeFlage::singlePrecision:
				zone_data.resize(_size, tec_data(_size, (float)0.0));
				break;

			case (char)dataTypeFlage::doublePrecision:
				zone_data.resize(_size, tec_data(_size, (double)0.0));
				break;

			default:
				std::cout << "ERROR!: could not resize the zone #" << zone+1;
				std::cout << " for \"" << name << "\"" << std::endl;
		}
		*/
		subzoneData.resize(_size);
	}
	
	//else {
		//delete zone_data[zone];
	//}
	
	switch((char)T) {
		case (char)dataTypeFlag::singlePrecision:
			subzoneData[zone].resize<float>(_size, (float)0.0);
			break;

		case (char)dataTypeFlag::doublePrecision:
			subzoneData[zone].resize<double>(_size, (double)0.0);
			break;

		default:
			std::cout << "ERROR!: could not resize the zone #" << zone+1;
			std::cout << " for \"" << name << "\"" << std::endl;
	}
}

tec_data& tec_variable::operator[](int zoneIdx) {
	return subzoneData[zoneIdx];
}

tec_zoneDetails::tec_zoneDetails() {}

tec_zoneDetails::~tec_zoneDetails() {}

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
			std::cout << "ERROR!: Unrecognized character used to set I, J, or K size" << std::endl;
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
		std::cout << "ERROR!: Unrecognized formatting type... current formatting is set to ";
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
		std::cout << "ERROR!: Unrecognized zone type... current zone type is set to ";
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

formattingFlag tec_zoneDetails::get_formattingType() {
	return dataPacking;
}

zoneTypeFlag tec_zoneDetails::get_zoneType() {
	return zoneType;
}

int tec_zoneDetails::get_size() {
	int s = I;
	if(J) {
		s *= J;
	}
	if(K) {
		s *= K;
	}
	return s;
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

