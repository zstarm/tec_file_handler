#include "tec_zoneInformation.h"

namespace tec {
	//--------------------------------------------------------------------------------------------
	// TECPLOT ZONE DETAILS
	//--------------------------------------------------------------------------------------------

	zoneInformation::zoneInformation(size_t vars) : nVars(vars), zoneTitle("ZONE 001"), hasSharedVars(false), hasPassiveVars(false) { zoneType = zoneTypeFlag::ordered; 
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

	zoneInformation::zoneInformation(int vars) : nVars(vars), zoneTitle("ZONE 001"), hasSharedVars(false), hasPassiveVars(false) {
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

	zoneInformation::zoneInformation(const zoneInformation &obj) {
		
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

	zoneInformation::zoneInformation(zoneInformation &&obj) {
		
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

	zoneInformation& zoneInformation::operator=(const zoneInformation &obj) {
		
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

		return *this;
}
	
	zoneInformation& zoneInformation::operator=(zoneInformation &&obj) {
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

		return *this;
	}

}
