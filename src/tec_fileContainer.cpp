#include "tec_fileContainer.h"

namespace tec {
	//--------------------------------------------------------------------------------------------
	// TECPLOT ZONE DETAILS
	//--------------------------------------------------------------------------------------------

	zoneInformation::zoneInformation(int zid, size_t vars) : nVars(vars), zoneID(zid), zoneTitle("ZONE 001"), hasSharedVars(false), hasPassiveVars(false) { zoneType = zoneTypeFlag::ordered; 
		dataPacking = formatFlag::point;
		I = 0;
		J = 0;
		K = 0;
		strandID = 0;
		solutionTime = 0.0;

		zone_sharedVars.resize(nVars);
		zone_passiveVars.resize(nVars);
		zone_varDTs.resize(nVars,1); //default to Float DT
		zone_varLoc.resize(nVars,1); //default to nodal location
	}

	zoneInformation::zoneInformation(int zid, int vars) : nVars(vars), zoneID(zid), zoneTitle("ZONE 001"), hasSharedVars(false), hasPassiveVars(false) {
		zoneType = zoneTypeFlag::ordered; 
		dataPacking = formatFlag::point;
		I = 0;
		J = 0;
		K = 0;
		strandID = 0;
		solutionTime = 0.0;
		nFaceConns = 0;
		faceConnectMode = faceConnMode::localone2one;
		shareConnZone = 0;

		zone_sharedVars.resize(nVars);
		zone_passiveVars.resize(nVars);
		zone_varDTs.resize(nVars,1); //default to Float DT
		zone_varLoc.resize(nVars,1); //default to nodal location
	}
	
	zoneInformation::~zoneInformation() {
	}

	zoneInformation::zoneInformation(zoneInformation &obj) : zoneID(obj.zoneID) {
		
		nVars = obj.nVars;

		I = obj.I;
		J = obj.J;
		K = obj.K;

		dataPacking = obj.dataPacking;
		zoneType = obj.zoneType;
		zoneTitle = obj.zoneTitle;

		strandID = obj.strandID;
		solutionTime = obj.solutionTime;
		
		nFaceConns = obj.nFaceConns;
		faceConnectMode = obj.faceConnectMode;
		shareConnZone = obj.shareConnZone;

		hasSharedVars = obj.hasSharedVars;
		hasPassiveVars = obj.hasPassiveVars;

		zone_varDTs = obj.zone_varDTs;
		zone_varLoc = obj.zone_varLoc;
		zone_sharedVars = obj.zone_sharedVars;
		zone_passiveVars = obj.zone_passiveVars;

	}

	zoneInformation::zoneInformation(zoneInformation &&obj) : zoneID(std::move(obj.zoneID)) {
		
		nVars = std::move(obj.nVars);

		I = std::move(obj.I);
		J = std::move(obj.J);
		K = std::move(obj.K);

		dataPacking = std::move(obj.dataPacking);
		zoneType = std::move(obj.zoneType);
		zoneTitle = std::move(obj.zoneTitle);

		strandID = std::move(obj.strandID);
		solutionTime = std::move(obj.solutionTime);
		
		nFaceConns = std::move(obj.nFaceConns);
		faceConnectMode = std::move(obj.faceConnectMode);
		shareConnZone = std::move(obj.shareConnZone);
		
		hasSharedVars = std::move(obj.hasSharedVars);
		hasPassiveVars = std::move(obj.hasPassiveVars);

		zone_varDTs = std::move(obj.zone_varDTs);
		zone_varLoc = std::move(obj.zone_varLoc);
		zone_sharedVars = std::move(obj.zone_sharedVars);
		zone_passiveVars = std::move(obj.zone_passiveVars);
	}

	void zoneInformation::set_IJKSize(char IJK, int size) {
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

	void zoneInformation::set_faceConns(int64_t numConnections, std::vector<int32_t> &faceConnects) {
		nFaceConns = numConnections;
		int32_faceConns = std::make_unique<std::vector<int32_t>>(std::move(faceConnects));
	}
	
	void zoneInformation::set_faceConns(int64_t numConnections, std::vector<int64_t> &faceConnects) {
		nFaceConns = numConnections;
		int64_faceConns = std::make_unique<std::vector<int64_t>>(std::move(faceConnects));
	}
	
	void zoneInformation::set_formatType(char formattingType) {
		if(formattingType == 'P' || formattingType == 'p') {
			dataPacking = formatFlag::point;
		}
		else if(formattingType == 'B' || formattingType == 'b') {
			dataPacking = formatFlag::block;
		}
		else {
			std::cout << "ERROR!: Unrecognized formatting type for set_formatType, current formatting is set to ";
			if(dataPacking == formatFlag::point) {
				std::cout << "\"POINT\"" << std::endl;
			}
			else {
				std::cout << "\"BLOCK\"" << std::endl;
			}
		}
	}

	void zoneInformation::set_zoneType(char type) {
		if(type == 'O' || type == 'o') {
			zoneType = zoneTypeFlag::ordered;
		}

		else if(type == 'F' || type == 'f') {
			//zoneType = zoneTypeFlag::FE;
			throw containerError("FINITE ELEMENT ZONES ARE NOT SUPPORTED");
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

	void zoneInformation::set_faceConnectMode(int32_t mode) {
		faceConnectMode = (faceConnMode)mode;
	}
	
	void zoneInformation::set_shareConnZone(int32_t shareZone) {
		shareConnZone = shareZone;
	}

	void zoneInformation::set_zoneTitle(std::string title) {
		zoneTitle = title;
	}

	void zoneInformation::set_strandID(int strand) {
		strandID = strand;
	}

	void zoneInformation::set_solutionTime(double time) {
		solutionTime = time;
	}

	void zoneInformation::set_varDT(int vidx, int32_t type, int resize) {
		if(vidx < nVars) {
			zone_varDTs[vidx] = type;
		}
		else {
			if(resize) {
				zone_varDTs.resize(resize, 1);
				if(zone_varLoc.size() != resize) {
					//resize the variable location as well to 1 (located a node)
					zone_varLoc.resize(resize, 1);
				}
				if(zone_sharedVars.size() != resize) {
					//resize the zone shared variables as well to 0 (nonshared variable id)
					zone_sharedVars.resize(resize, 0);
				}
				if(zone_passiveVars.size() != resize) {
					//resize the zone passive variables as well to 0 (active variable id)
					zone_passiveVars.resize(resize, 0);
				}
				zone_varDTs[vidx] = type;
				nVars = resize;
			}
			else {
				throw containerError("variable index is out of range for setting data type!");
			}
		}
	}

	void zoneInformation::set_varLoc(int vidx, int32_t loc, int resize) {
		if(vidx < nVars) {
			zone_varLoc[vidx] = loc;
		}
		else {
			if(resize) {
				zone_varLoc.resize(resize, 1);
				if(zone_varDTs.size() != resize) {
					//resize the zone var DTs as well to 1 (float datatype)
					zone_varDTs.resize(resize, 1);
				}
				if(zone_sharedVars.size() != resize) {
					//resize the zone shared variables as well to 0 (nonshared variable id)
					zone_sharedVars.resize(resize, 0);
				}
				if(zone_passiveVars.size() != resize) {
					//resize the zone passive variables as well to 0 (active variable id)
					zone_passiveVars.resize(resize, 0);
				}
				zone_varLoc[vidx] = loc;
				nVars = resize;
			}
			else {
				throw containerError("variable index is out of range for setting value location!");
			}
		}
	}
	
	void zoneInformation::set_sharedVar(int vidx, int32_t zidx, int resize) {
		if(vidx < nVars) {
			zone_sharedVars[vidx] = zidx;
			if(zidx && !hasSharedVars) {
				hasPassiveVars = true;	
			}
			else if(!zidx && hasSharedVars) {
				//check to see if other passive vars exist before changing flag
				for(int v = 0; v < nVars; v++) {
					if(zone_sharedVars[v]) {
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
				zone_sharedVars.resize(resize, 0);
				if(zone_varLoc.size() != resize) {
					//resize the variable location as well to 1 (located a node)
					zone_varLoc.resize(resize, 1);
				}
				if(zone_passiveVars.size() != resize) {
					//resize the zone passive variables as well to 0 (active variable id)
					zone_passiveVars.resize(resize, 0);
				}
				if(zone_varDTs.size() != resize) {
					//resize the zone var DT vector as well to 1 (single precision id)
					zone_varDTs.resize(resize, 1);
				}
				zone_sharedVars[vidx] = zidx;
				nVars = resize;
				if(zidx && !hasSharedVars) {
					hasSharedVars = true;
				}
			}
			else {
				throw containerError("variable index is out of range for setting shared flag!");
			}
		}
	}

	void zoneInformation::set_passiveVar(int vidx, bool flag, int resize) {
		if(vidx < nVars) {
			zone_passiveVars[vidx] = flag;
			if(flag && !hasPassiveVars) {
				hasPassiveVars = true;	
			}
			else if(!flag && hasPassiveVars) {
				//check to see if other passive vars exist before changing flag
				for(int v = 0; v < nVars; v++) {
					if(zone_passiveVars[v]) {
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
				zone_passiveVars.resize(resize, false);
				if(zone_varLoc.size() != resize) {
					//resize the variable location as well to 1 (located a node)
					zone_varLoc.resize(resize, 1);
				}
				if(zone_sharedVars.size() != resize) {
					//resize the shared zone vector as well to 0 (nonshared variable id)
					zone_sharedVars.resize(resize, 0);
				}
				if(zone_varDTs.size() != resize) {
					//resize the zone var DT vector as well to 1 (single precision id)
					zone_varDTs.resize(resize, 1);
				}
				zone_passiveVars[vidx] = flag;
				nVars = resize;
				if(flag && !hasPassiveVars) {
					hasPassiveVars = true;
				}
			}
			else {
				throw containerError("variable index is out of range for setting passive flag!");
			}
		}
	}

	formatFlag zoneInformation::get_formattingType() {
		return dataPacking;
	}

	zoneTypeFlag zoneInformation::get_zoneType() {
		return zoneType;
	}

	std::string zoneInformation::get_zoneTitle() {
		return zoneTitle;
	}

	int64_t zoneInformation::get_numFaceConns() {
		return nFaceConns;
	}

	int32_t zoneInformation::get_shareConnZone() {
		return shareConnZone;
	}

	faceConnMode zoneInformation::get_faceConnectMode() {
		return faceConnectMode;
	}

	int32_t zoneInformation::get_strandID() {
		return strandID;
	}

	double zoneInformation::get_solutionTime() {
		return solutionTime;
	}

	int zoneInformation::get_size(bool node) {
		if(I || J || K) {
			int s = 1;

			if(I > 1) {
				//if at node use I, if cell centered, get I-1
				node ? s*= I : s*= (I-1);
			}
			if(J > 1) {
				//if at node use J, if cell centered, get J-1
				node ? s*= J : s*= (J-1);
			}
			if(K > 1) {
				//if at node use K, if cell centered, get K-1
				node ? s*= K : s*= (K-1);
			}

			return s;
		}
		return 0;
	}

	int64_t zoneInformation::get_Imax() {
		return I;
	}

	int64_t zoneInformation::get_Jmax() {
		return J;
	}

	int64_t zoneInformation::get_Kmax() {
		return K;
	}

	std::unique_ptr<std::vector<int32_t>> zoneInformation::get_varDTs() {
		if(!zone_varDTs.size()) {
			throw containerError("variable type vector is empty!");
		}
		return std::make_unique<std::vector<int32_t>>(zone_varDTs);
	}

	std::unique_ptr<std::vector<int32_t>> zoneInformation::get_sharedList() {
		if(!zone_sharedVars.size()) {
			return NULL;
		}
		return std::make_unique<std::vector<int32_t>>(zone_sharedVars);
	}

	std::unique_ptr<std::vector<int32_t>>  zoneInformation::get_locationList() {
		if(!zone_varLoc.size()) {
			std::cout << "no size" << std::endl;
			return NULL;
		}
		return std::make_unique<std::vector<int32_t>>(zone_varLoc);
	}
	
	std::unique_ptr<std::vector<int32_t>>  zoneInformation::get_passiveList() {
		if(!zone_passiveVars.size()) {
			return NULL;
		}
		return std::make_unique<std::vector<int32_t>>(zone_passiveVars);
	}

	//----------------------------------------------------------------------------------------------
	// TECPLOT DATA
	//----------------------------------------------------------------------------------------------
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
		
	template<typename DT> 
	void zoneData::resize(int new_size, DT val) {
		
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
				std::cout << "ERROR!: unrecognized data type for zoneData, defaulting to \"float\" data type" << std::endl;
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

	//--------------------------------------------------------------------------------------------
	// TECPLOT VARIABLE
	//--------------------------------------------------------------------------------------------
	variable::variable() : name("V") {}

	variable::variable(std::string vname) : name(vname) {}

	variable::variable(variable &obj) : subzoneData(obj.subzoneData) {
		name = obj.name;
	}

	variable::variable(variable &&obj) : subzoneData(std::move(obj.subzoneData)){
		name = std::move(obj.name);
	}

	variable::~variable() {
	}

	void variable::resize_zone(int zone, int _size, dataTypeFlag T) {
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
		throw containerError("zone index exceeds subzone vector limits");
	}


	//--------------------------------------------------------------------------------------------
	// TECPLOT FILE CONTENT
	//--------------------------------------------------------------------------------------------
	fileContainer::fileContainer() : fType(fileTypeFlag::full), title("N/A") {}

	fileContainer::~fileContainer() {}

	void fileContainer::print_headerDetails() {
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
		std::cout << "TECPLOT FILE HEADER DETAILS" << std::endl;
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
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
		std::cout << "VARIABLES (#SZ): ";
		for(int v = 0; v < vars.size(); v++) {
			std::cout << vars[v].name;
			std::cout << "(" << vars[v].subzoneData.size() << ")";
			//ternary operator to determine if at the end of the variable list or not
			v != vars.size()-1 ? std::cout << ", " : std::cout << std::endl;
		}
	}

	void fileContainer::print_zoneDetails(int zidx) {
		std::cout << "TECPLOT ZONE #" << zoneDetails[zidx].zoneID << " DETAILS" << std::endl;
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
		
		//zone title
		std::cout << "ZONE TITLE: " << zoneDetails[zidx].zoneTitle << std::endl;

		//zone type
		std::cout << "ZONE TYPE: ";
		zoneDetails[zidx].zoneType == zoneTypeFlag::ordered ? std::cout << "ORDERED" : std::cout << "FINITE ELEMENT";
		std::cout << std::endl;

		//zone formatting/datapacking 
		std::cout << "ZONE FORMATTING: ";
		zoneDetails[zidx].dataPacking == formatFlag::point ? std::cout << "POINT" : std::cout << "BLOCK";
		std::cout << std::endl;

		std::cout << "ZONE DIMENSIONS: I = " << zoneDetails[zidx].I << ", J = " << zoneDetails[zidx].J << ", K = " << zoneDetails[zidx].K << std::endl;

		//transient datasets only
		if(zoneDetails[zidx].strandID) {
			//strand ID
			std::cout << "ZONE STRAND ID: " << zoneDetails[zidx].strandID << std::endl;
			std::cout << "ZONE SOLUTION TIME: " << zoneDetails[zidx].solutionTime << std::endl;
		}

	}

	void fileContainer::print_zoneData(int zidx) {
		std::cout << "TECPLOT ZONE #" << zoneDetails[zidx].zoneID << " DATA" << std::endl;
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
		for(int v = 0; v < vars.size(); v++) {
			std::cout << vars[v].name;
			std::cout << "(";
			if(zoneDetails[zidx].zone_sharedVars[v]) {
				std::cout << "SHARED)";
				//ternary operator to determine if at the end of the variable list or not
				v != vars.size()-1 ? std::cout << ", " : std::cout << std::endl;
				continue;
			}
			else if(zoneDetails[zidx].zone_passiveVars[v]) {
				std::cout << "PASSIVE)";
				//ternary operator to determine if at the end of the variable list or not
				v != vars.size()-1 ? std::cout << ", " : std::cout << std::endl;
				continue;
			}
			switch(vars[v][zidx].T) {
				case dataTypeFlag::singlePrecision:
					std::cout << "SINGLE, ";
					break;
				case dataTypeFlag::doublePrecision:
					std::cout << "DOUBLE, ";
					break;
				case dataTypeFlag::int32:
					std::cout << "LONGINT, ";
					break;
				case dataTypeFlag::int16:
					std::cout << "SHORTINT, ";
					break;
				case dataTypeFlag::byte:
					std::cout << "BYTE, ";
					break;
				default:
					throw containerError("issue getting data type for variable: " + vars[v].name + " at zone" + std::to_string(zidx) + " to display");
			}
			if(zoneDetails[zidx].zone_varLoc[v]) {
				std::cout << "NODE";
			}
			else {
				std::cout << "CELLCENTER";
			}
			std::cout << ")";
			//ternary operator to determine if at the end of the variable list or not
			v != vars.size()-1 ? std::cout << ", " : std::cout << std::endl;
		}
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
		for(int i = 0; i < zoneDetails[zidx].get_size(); i++) {
			for(int v = 0; v < vars.size(); v++) {
				if(zoneDetails[zidx].zone_sharedVars[v]) {
					if(!v) {
						continue;
					}
					v != vars.size()-1 ? std::cout << "\t" : std::cout << std::endl;
					continue;
				}
				else if(zoneDetails[zidx].zone_passiveVars[v]) {
					if(!v) {
						continue;
					}
					v != vars.size()-1 ? std::cout << "\t" : std::cout << std::endl;
					continue;
				}
				
				if(!zoneDetails[zidx].zone_varLoc[v]) {
					//if cell centered make sure index isn't above number of cell values
					int nCells = zoneDetails[zidx].get_size(false);
					if(i >= nCells) {
						if(!v) {
							continue;
						}
						v != vars.size()-1 ? std::cout << "\t" : std::cout << std::endl;
						continue;
					}
				}
				
				switch(vars[v][zidx].T) {
					case dataTypeFlag::singlePrecision:
						std::cout << vars[v][zidx].get_float(i);
						break;
					case dataTypeFlag::doublePrecision:
						std::cout << vars[v][zidx].get_double(i);
						break;
					case dataTypeFlag::int32:
						std::cout << vars[v][zidx].get_int32(i);
						break;
					case dataTypeFlag::int16:
						std::cout << vars[v][zidx].get_int16(i);
						break;
					case dataTypeFlag::byte:
						std::cout << vars[v][zidx].get_byte(i);
						break;
					default:
						throw containerError("issue getting data type for variable: " + vars[v].name + " at zone" + std::to_string(zidx) + " to display");
				}
				//ternary operator to determine if at the end of the variable list or not
				v != vars.size()-1 ? std::cout << "\t" : std::cout << std::endl;
			}
		}
	}

	void fileContainer::print_fileDetails(bool include_data) {
		try {
			print_headerDetails();
			std::cout << "\n" << std::endl;	
			for(int z = 0; z < zoneDetails.size(); z++) {
				print_zoneDetails(z);
				std::cout << "\n" << std::endl;	
				if(include_data) {
					print_zoneData(z);
					std::cout << "\n" << std::endl;	
				}
			}
		}
		catch(containerError const &e) {
			std::cout << "\nCONTAINER ERROR: " << e.what() << " (CODE: " << e.code + e.secondary_code << ")" << std::endl;
		}
	}

	void fileContainer::operator[](int vidx) {
		std::cout << "variable name at " << vidx << " is " << vars[vidx].name << std::endl;
	}

	void fileContainer::operator[](std::string vname) {
		try {
			int vidx = var_map.at(vname);
			std::cout << "variable name given key " << vname << " is " << vars[vidx].name << std::endl;
		}

		catch(std::out_of_range const& e) {
			std::cout << "The variable key \"" << vname << "\" does not correspond to the name any variable" << std::endl;
			std::cout << "Please using one of the following names as a key or the corrsponding index number:" << std::endl;
			for(int v = 0; v < vars.size(); v++) {
				std::cout << v << ": ";
				std::cout << vars[v].name << std::endl;
			}
		}
		
	}
}
