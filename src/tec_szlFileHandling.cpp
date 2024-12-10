#include "tec_szlFileHandling.h"

namespace tec {
	
	szlReader::szlReader() : nZones(0), nVars(0), szlFileHandle(NULL) {}
	
	szlReader::szlReader(std::string _fname) : nZones(0), nVars(0), szlFileHandle(NULL), fname(_fname) {}
	
	szlReader::szlReader(const char* _fname) : nZones(0), nVars(0), szlFileHandle(NULL), fname(_fname) {}

	szlReader::~szlReader() {}

	void szlReader::read_currentZone(int zidx, fileContent &dataContainer, int &err) {
		dataContainer.zoneDetails.push_back({zidx+1, nVars}); //pushback new instance of zone
		
		//get zone title
		char* tmpZoneTitle = NULL;
		err = tecZoneGetTitle(szlFileHandle, zidx+1, &tmpZoneTitle);
		dataContainer.zoneDetails[zidx].set_zoneTitle(std::string(tmpZoneTitle));

		//get zone type
		int tmpZoneType;
		err = tecZoneGetType(szlFileHandle, zidx+1, &tmpZoneType);
		if(!tmpZoneType) {
			dataContainer.zoneDetails[zidx].set_zoneType('O'); //set ordered type
		}
		else {
			dataContainer.zoneDetails[zidx].set_zoneType('F'); //set FE type
		}

		//get zone dimensions (i.e. I,J,K)
		int64_t tmpI, tmpJ, tmpK;
		err = tecZoneGetIJK(szlFileHandle, zidx+1, &tmpI, &tmpJ, &tmpK);
		dataContainer.zoneDetails[zidx].set_IJKSize('I', tmpI);
		dataContainer.zoneDetails[zidx].set_IJKSize('J', tmpJ);
		dataContainer.zoneDetails[zidx].set_IJKSize('K', tmpK);

		int zoneSize = tmpI*tmpJ*tmpK;

		//get zone strandID
		err = tecZoneGetStrandID(szlFileHandle, zidx+1, &tmpZoneType); //reusing tmpZoneType var
		dataContainer.zoneDetails[zidx].set_strandID(tmpZoneType);

		//get zone solution time
		double tmpZoneSolutionTime;
		err = tecZoneGetSolutionTime(szlFileHandle, zidx+1, &tmpZoneSolutionTime);
		dataContainer.zoneDetails[zidx].set_solutionTime(tmpZoneSolutionTime);

		//check variables for data type and if shared or passive for zone
		for(int v = 0; v < nVars; v++) {
			int32_t tmpDataType;
			int32_t tmpSourceZone;
			int32_t tmpPassiveFlag;

			err = tecZoneVarGetType(szlFileHandle, zidx+1, v+1, &tmpDataType);	
			err = tecZoneVarGetSharedZone(szlFileHandle, zidx+1, v+1, &tmpSourceZone);
			err = tecZoneVarIsPassive(szlFileHandle, zidx+1, v+1, &tmpPassiveFlag);	
			
			dataContainer.zoneDetails[zidx].set_varDT(v, tmpDataType); //set var type

			if(tmpPassiveFlag) {
				//if passive, update the vector to have passive flag = true
				dataContainer.zoneDetails[zidx].set_passiveVar(v, true);
			}
			else if(tmpSourceZone) {
				//if not passive, check if shared, 
				//update the vector with source zone if true
				dataContainer.zoneDetails[zidx].set_sharedVar(v, tmpSourceZone);
			}

			if(!tmpPassiveFlag && !tmpSourceZone) {
				//if neither passive nor shared,
				//allocate subzone data space
				dataContainer.variables[v].resize_zone(zidx, zoneSize, (dataTypeFlag)tmpDataType);
				insert_zoneData(zidx, v, zoneSize, dataContainer.variables[v][zidx]);
			}
		}
	}

	void szlReader::insert_zoneData(int zidx, int vidx, int nValues, zoneData &subzoneData) {
		
		switch(subzoneData.type()) {
			case dataTypeFlag::singlePrecision:
				tecZoneVarGetFloatValues(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_float(0));
				break;
			
			case dataTypeFlag::doublePrecision:
				tecZoneVarGetDoubleValues(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_double(0));
				break;

			case dataTypeFlag::int32:
				tecZoneVarGetInt32Values(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_int32(0));
				break;

			case dataTypeFlag::int16:
				tecZoneVarGetInt16Values(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_int16(0));
				break;
			
			case dataTypeFlag::byte:
				tecZoneVarGetUInt8Values(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_byte(0));
				break;

			//default:

		}

	}


	void szlReader::read_file(fileContent &dataContainer) {
		int err; //var to store error code
		
		//open the file 
		err = tecFileReaderOpen(fname.c_str(), &szlFileHandle);

		//-------------------------------------------------
		// GET DATASET FILE HEADER INFO
		//-------------------------------------------------
		char* tmpCharPtr = NULL;
		err = tecDataSetGetTitle(szlFileHandle, &tmpCharPtr); //title
		dataContainer.title = tmpCharPtr;

		err = tecDataSetGetNumVars(szlFileHandle, &nVars); //num of vars
		for(int v = 0; v < nVars; v++) {
			tmpCharPtr = NULL;
			err = tecVarGetName(szlFileHandle, v+1, &tmpCharPtr);	 
			dataContainer.variables.push_back(std::string(tmpCharPtr)); //var names
			dataContainer.var_map[std::string(tmpCharPtr)] = v;		
		}
	
		int32_t tmpType;
		err = tecFileGetType(szlFileHandle, &tmpType);
		dataContainer.fType = (fileTypeFlag)tmpType;
		
		//-------------------------------------------------
		// GET DATASET SUBZONE INFO
		//-------------------------------------------------
		err = tecDataSetGetNumZones(szlFileHandle, &nZones);
		for(int z = 0; z < nZones; z++) {
			read_currentZone(z, dataContainer, err);
		}

		//close file
		tecFileReaderClose(&szlFileHandle);

	}

	void szlReader::read_file(std::string _fname, fileContent &dataContainer) {
		fname = _fname;
		read_file(dataContainer);
	}
}

