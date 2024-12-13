#include "tec_szlFileHandling.h"

#define SZPLT_FORMAT 1

namespace tec {

	//--------------------------------------------------------------------------------------------
	// SZL FILE READER
	//--------------------------------------------------------------------------------------------
	
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
		tecStringFree(&tmpZoneTitle);

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
		err = tecZoneGetIJK(szlFileHandle, zidx+1, 
				&dataContainer.zoneDetails[zidx].I,
				&dataContainer.zoneDetails[zidx].J,
				&dataContainer.zoneDetails[zidx].K);

		int zoneSize = dataContainer.zoneDetails[zidx].get_size();

		//get zone strandID
		err = tecZoneGetStrandID(szlFileHandle, zidx+1, &tmpZoneType); //reusing tmpZoneType var
		dataContainer.zoneDetails[zidx].set_strandID(tmpZoneType);

		//get zone solution time
		double tmpZoneSolutionTime;
		err = tecZoneGetSolutionTime(szlFileHandle, zidx+1, &tmpZoneSolutionTime);
		dataContainer.zoneDetails[zidx].set_solutionTime(tmpZoneSolutionTime);

		//get connectivity share zone 
		err = tecZoneConnectivityGetSharedZone(szlFileHandle, zidx+1, &tmpZoneType); //reusing tmpZoneType
		if(tmpZoneType) {
			//if zone shares connectivity information
			dataContainer.zoneDetails[zidx].set_shareConnectivityZone(tmpZoneType);
		}

		int64_t tmpNumFaceConnections;
		err = tecZoneFaceNbrGetNumConnections(szlFileHandle, zidx+1, &tmpNumFaceConnections);
		if(tmpNumFaceConnections) {
			//if there are face connections in the zone	
			err = tecZoneFaceNbrGetMode(szlFileHandle, zidx+1, &tmpZoneType); //reusing tmpZoneType
			dataContainer.zoneDetails[zidx].set_faceConnectionMode(tmpZoneType);

			err = tecZoneFaceNbrsAre64Bit(szlFileHandle, zidx+1, &tmpZoneType); //reusing tmpZoneType
			int64_t tmpNumFaceValues;	
			err = tecZoneFaceNbrGetNumValues(szlFileHandle, zidx+1, &tmpNumFaceValues);
			if(tmpZoneType) {
				//face connection values are 64 bit
				std::vector<int64_t> faceConnections(tmpNumFaceValues);
				err = tecZoneFaceNbrGetConnections64(szlFileHandle, zidx+1, &faceConnections[0]);
				dataContainer.zoneDetails[zidx].set_faceConnections(tmpNumFaceConnections, faceConnections);
			}
			else {
				//face connection values are 32 bit
				std::vector<int32_t> faceConnections(tmpNumFaceValues);
				err = tecZoneFaceNbrGetConnections(szlFileHandle, zidx+1, &faceConnections[0]);
				dataContainer.zoneDetails[zidx].set_faceConnections(tmpNumFaceConnections, faceConnections);
			}

		}

		//check variables for data type and if shared or passive for zone
		for(int v = 0; v < nVars; v++) {
			int32_t tmpDataType;
			int32_t tmpSourceZone;
			int32_t tmpPassiveFlag;
			int32_t tmpValueLocation;

			err = tecZoneVarGetType(szlFileHandle, zidx+1, v+1, &tmpDataType);	
			err = tecZoneVarGetSharedZone(szlFileHandle, zidx+1, v+1, &tmpSourceZone);
			err = tecZoneVarGetValueLocation(szlFileHandle, zidx+1, v+1, &tmpValueLocation);
			err = tecZoneVarIsPassive(szlFileHandle, zidx+1, v+1, &tmpPassiveFlag);	
			
			dataContainer.zoneDetails[zidx].set_varDT(v, tmpDataType); //set var type
			dataContainer.zoneDetails[zidx].set_varLoc(v, tmpValueLocation); //set value location

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
		
		//open the file for reading
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

	//--------------------------------------------------------------------------------------------
	// SZL FILE WRITER
	//--------------------------------------------------------------------------------------------

	szlWriter::szlWriter() : szlFileHandle(NULL) {}
	
	szlWriter::szlWriter(std::string _fname) : szlFileHandle(NULL), fname(_fname) {}
	
	szlWriter::szlWriter(const char* _fname) : szlFileHandle(NULL), fname(_fname) {}

	szlWriter::~szlWriter() {}

	void szlWriter::write_file(fileContent &dataContainer) {
		int err; //var to store error code
		
		//get file header information
		std::string varlist;
		int nVars = dataContainer.variables.size();
		int nZones = dataContainer.zoneDetails.size();
		for(int v = 0; v < nVars; v++) {
			varlist += dataContainer.variables[v].get_name();	
			if(v != nVars-1) { varlist+=",";}
		}
		
		err = tecFileWriterOpen(fname.c_str(), dataContainer.title.c_str(), varlist.c_str(), 
				SZPLT_FORMAT, (int32_t)dataContainer.fType, 0, NULL, &szlFileHandle);
		
		for(int z = 0; z < nZones; z++) {
			int32_t zType = (int32_t)dataContainer.zoneDetails[z].get_zoneType();
			std::string zTitle = dataContainer.zoneDetails[z].get_zoneTitle();
			auto varDataTypes = dataContainer.zoneDetails[z].get_varDTs();
			auto sharingSource = dataContainer.zoneDetails[z].get_sharedList();
			auto valueLocation = dataContainer.zoneDetails[z].get_locationList();
			auto passiveVars = dataContainer.zoneDetails[z].get_passiveList();

		}
		

		err = tecFileWriterClose(&szlFileHandle);

		//-------------------------------------------------
		// GET DATASET FILE HEADER INFO
		//-------------------------------------------------

	}
	
	void szlWriter::write_file(std::string _fname, fileContent &dataContainer) {
		fname = _fname;
		write_file(dataContainer);
	}

}

