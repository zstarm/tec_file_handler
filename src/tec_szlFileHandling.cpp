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

	void szlReader::read_currentZone(int zidx, fileContainer &dataContainer, int &err) {
		dataContainer.zoneDetails.push_back({zidx+1, nVars}); //pushback new instance of zone
		
		//get zone title
		char* tmpZoneTitle = NULL;
		err = tecZoneGetTitle(szlFileHandle, zidx+1, &tmpZoneTitle);
		if(err) {
			throw szlReaderError("TECIO error getting zone " + std::to_string(zidx+1) + " title", 1);
		}
		dataContainer.zoneDetails[zidx].set_zoneTitle(std::string(tmpZoneTitle));
		tecStringFree(&tmpZoneTitle);

		//get zone type
		int tmpZoneType;
		err = tecZoneGetType(szlFileHandle, zidx+1, &tmpZoneType);
		if(err) {
			throw szlReaderError("TECIO error getting zone " + std::to_string(zidx+1) + "type", 1);
		}
		if(!tmpZoneType) {
			dataContainer.zoneDetails[zidx].set_zoneType('O'); //set ordered type
		}
		else {
			throw szlReaderError("Unsupported zone type", 2);
			//dataContainer.zoneDetails[zidx].set_zoneType('F'); //set FE type
		}

		//get zone dimensions (i.e. I,J,K)
		err = tecZoneGetIJK(szlFileHandle, zidx+1, 
				&dataContainer.zoneDetails[zidx].I,
				&dataContainer.zoneDetails[zidx].J,
				&dataContainer.zoneDetails[zidx].K);
		if(err) {
			throw szlReaderError("TECIO error getting dimensions of zone " + std::to_string(zidx+1), 1);
		}
		//int zoneSize = dataContainer.zoneDetails[zidx].get_size();

		//get zone strandID
		err = tecZoneGetStrandID(szlFileHandle, zidx+1, &tmpZoneType); //reusing tmpZoneType var
		if(err) {
			throw szlReaderError("TECIO error getting strandID of zone " + std::to_string(zidx+1), 1);
		}
		dataContainer.zoneDetails[zidx].set_strandID(tmpZoneType);

		//get zone solution time
		double tmpZoneSolutionTime;
		err = tecZoneGetSolutionTime(szlFileHandle, zidx+1, &tmpZoneSolutionTime);
		if(err) {
			throw szlReaderError("TECIO error getting solution time of zone " + std::to_string(zidx+1), 1);
		}
		dataContainer.zoneDetails[zidx].set_solutionTime(tmpZoneSolutionTime);

		//get connectivity share zone 
		err = tecZoneConnectivityGetSharedZone(szlFileHandle, zidx+1, &tmpZoneType); //reusing tmpZoneType
		if(err) {
			throw szlReaderError("TECIO error connectivity share zone for zone " + std::to_string(zidx+1), 1);
		}
		if(tmpZoneType) {
			//if zone shares connectivity information
			dataContainer.zoneDetails[zidx].set_shareConnZone(tmpZoneType);
		}

		int64_t tmpNumFaceConnections;
		err = tecZoneFaceNbrGetNumConnections(szlFileHandle, zidx+1, &tmpNumFaceConnections);
		if(err) {
			throw szlReaderError("TECIO error getting number of face neighbor connections for zone " + std::to_string(zidx+1), 1);
		}
		if(tmpNumFaceConnections) {
			//if there are face connections in the zone	
			err = tecZoneFaceNbrGetMode(szlFileHandle, zidx+1, &tmpZoneType); //reusing tmpZoneType
			if(err) {
				throw szlReaderError("TECIO error getting zone " + std::to_string(zidx+1) + " face neighbor mode", 1);
			}
			dataContainer.zoneDetails[zidx].set_faceConnectMode(tmpZoneType);
			err = tecZoneFaceNbrsAre64Bit(szlFileHandle, zidx+1, &tmpZoneType); //reusing tmpZoneType
			if(err) {
				throw szlReaderError("TECIO error determining if zone " + std::to_string(zidx+1) + " face neighbor values are 64 bit", 1);
			}
			int64_t tmpNumFaceValues;	
			err = tecZoneFaceNbrGetNumValues(szlFileHandle, zidx+1, &tmpNumFaceValues);
			if(err) {
				throw szlReaderError("TECIO error getting number of zone " + std::to_string(zidx+1) + " face neighbor values", 1);
			}
			if(tmpZoneType) {
				//face connection values are 64 bit
				std::vector<int64_t> faceConnections(tmpNumFaceValues);
				err = tecZoneFaceNbrGetConnections64(szlFileHandle, zidx+1, &faceConnections[0]);
				if(err) {
					throw szlReaderError("TECIO error getting zone " + std::to_string(zidx+1) + " face neighbor connections (64 bit)", 1);
				}
				dataContainer.zoneDetails[zidx].set_faceConns(tmpNumFaceConnections, faceConnections);
			}
			else {
				//face connection values are 32 bit
				std::vector<int32_t> faceConnections(tmpNumFaceValues);
				err = tecZoneFaceNbrGetConnections(szlFileHandle, zidx+1, &faceConnections[0]);
				if(err) {
					throw szlReaderError("TECIO error getting zone " + std::to_string(zidx+1) + " face neighbor connections", 1);
				}
				dataContainer.zoneDetails[zidx].set_faceConns(tmpNumFaceConnections, faceConnections);
			}
		}
		//check variables for data type and if shared or passive for zone
		for(int v = 0; v < nVars; v++) {
			int32_t tmpDataType;
			int32_t tmpSourceZone;
			int32_t tmpPassiveFlag;
			int32_t tmpValueLocation;

			err = tecZoneVarGetType(szlFileHandle, zidx+1, v+1, &tmpDataType);	
			if(err) {
				throw szlReaderError("TECIO error getting variable types in zone " + std::to_string(zidx+1), 1);
			}
			err = tecZoneVarGetSharedZone(szlFileHandle, zidx+1, v+1, &tmpSourceZone);
			if(err) {
				throw szlReaderError("TECIO error getting variable share in zone " + std::to_string(zidx+1), 1);
			}
			err = tecZoneVarGetValueLocation(szlFileHandle, zidx+1, v+1, &tmpValueLocation);
			if(err) {
				throw szlReaderError("TECIO error getting variable value location in zone " + std::to_string(zidx+1), 1);
			}
			err = tecZoneVarIsPassive(szlFileHandle, zidx+1, v+1, &tmpPassiveFlag);	
			if(err) {
				throw szlReaderError("TECIO error getting variable passive flag in zone" + std::to_string(zidx+1), 1);
			}
			
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
				int64_t zoneSize;
				err = tecZoneVarGetNumValues(szlFileHandle, zidx+1, v+1, &zoneSize);
				if(err) {
					throw szlReaderError("TECIO error getting zone size for zone " + std::to_string(zidx+1), 1);
				}
				dataContainer.vars[v].resize_zone(zidx, (int)zoneSize, (dataTypeFlag)tmpDataType);
				insert_zoneData(zidx, v, zoneSize, dataContainer.vars[v][zidx]);
			}
		}
	}

	void szlReader::insert_zoneData(int zidx, int vidx, int nValues, zoneData &subzoneData) {
		int err;
		switch(subzoneData.type()) {
			case dataTypeFlag::singlePrecision:
				err = tecZoneVarGetFloatValues(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_float(0));
				break;
			
			case dataTypeFlag::doublePrecision:
				err = tecZoneVarGetDoubleValues(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_double(0));
				break;

			case dataTypeFlag::int32:
				err = tecZoneVarGetInt32Values(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_int32(0));
				break;

			case dataTypeFlag::int16:
				err = tecZoneVarGetInt16Values(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_int16(0));
				break;
			
			case dataTypeFlag::byte:
				err = tecZoneVarGetUInt8Values(szlFileHandle, zidx+1, vidx+1, 1, 
						nValues, &subzoneData.get_byte(0));
				break;

			default:
				throw szlReaderError("unrecognized dataTypeFlag when reading szl file data for zone " + std::to_string(zidx+1), 2);
		}
		if(err) {
			throw szlReaderError("TECIO error when reading data for zone " + std::to_string(zidx+1), 1);
		}
	}

	void szlReader::read_file(fileContainer &dataContainer) {
		int err; //var to store error code
		try {
			//open the file for reading
			err = tecFileReaderOpen(fname.c_str(), &szlFileHandle);
			if(err) {
				throw szlReaderError("TECIO error opening file \"" + fname, 1);
			}
			//-------------------------------------------------
			// GET DATASET FILE HEADER INFO
			//-------------------------------------------------
			char* tmpCharPtr = NULL;
			err = tecDataSetGetTitle(szlFileHandle, &tmpCharPtr); //title
			if(err) {
				throw szlReaderError("TECIO error getting file title", 1);
			}
			dataContainer.title = tmpCharPtr;

			err = tecDataSetGetNumVars(szlFileHandle, &nVars); //num of vars
			if(err) {
				throw szlReaderError("TECIO error getting number of variables", 1);
			}
			for(int v = 0; v < nVars; v++) {
				tmpCharPtr = NULL;
				err = tecVarGetName(szlFileHandle, v+1, &tmpCharPtr);	 
				if(err) {
					throw szlReaderError("TECIO error getting variable name", 1);
				}
				dataContainer.vars.push_back(std::string(tmpCharPtr)); //var names
				dataContainer.var_map[std::string(tmpCharPtr)] = v;		
			}
		
			int32_t tmpType;
			err = tecFileGetType(szlFileHandle, &tmpType);
			if(err) {
				throw szlReaderError("TECIO error getting file type", 1);
			}
			dataContainer.fType = (fileTypeFlag)tmpType;
			
			//-------------------------------------------------
			// GET DATASET SUBZONE INFO
			//-------------------------------------------------
			err = tecDataSetGetNumZones(szlFileHandle, &nZones);
			for(int z = 0; z < nZones; z++) {
				read_currentZone(z, dataContainer, err);
			}

		}
		
		catch(szlReaderError const &e) {
			std::cout << "\nSZL READING ERROR: " << e.what() << " (CODE: " << e.code+e.secondary_code << ")" << std::endl;
		}
		
		//close file
		err = tecFileReaderClose(&szlFileHandle);
		if(err) {
			throw szlReaderError("TECIO error closing file", 1);
		}
	}

	void szlReader::read_file(std::string _fname, fileContainer &dataContainer) {
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

	void szlWriter::write_currentZone(int zidx, fileContainer &dataContainer, int &nVars, int &err) {
		int32_t outZoneIdx;
		int32_t zType = (int32_t)dataContainer.zoneDetails[zidx].get_zoneType();
		std::string zTitle = dataContainer.zoneDetails[zidx].get_zoneTitle();
		int32_t timeStrand = dataContainer.zoneDetails[zidx].get_strandID();
		auto varDataTypes = dataContainer.zoneDetails[zidx].get_varDTs();
		auto sharingSource = dataContainer.zoneDetails[zidx].get_sharedList();
		auto valueLocation = dataContainer.zoneDetails[zidx].get_locationList();
		auto passiveVars = dataContainer.zoneDetails[zidx].get_passiveList();
		int32_t connectShareSource = dataContainer.zoneDetails[zidx].get_shareConnZone();
		int64_t numFaceConnects = dataContainer.zoneDetails[zidx].get_numFaceConns();
		int32_t faceConnectMode = (int32_t)dataContainer.zoneDetails[zidx].get_faceConnectMode();

		if(!(int32_t)dataContainer.zoneDetails[zidx].get_zoneType()) {
			//Ordered dataset -> createIJK zone
			err = tecZoneCreateIJK(szlFileHandle, zTitle.c_str(), dataContainer.zoneDetails[zidx].get_Imax(), dataContainer.zoneDetails[zidx].get_Jmax(), dataContainer.zoneDetails[zidx].get_Kmax(), varDataTypes->data(), sharingSource->data(), valueLocation->data(), &(*passiveVars)[0], connectShareSource, numFaceConnects, faceConnectMode, &outZoneIdx);
			if(err) {
				throw szlWriterError("TECIO error creating ordered zone " + std::to_string(zidx+1),1);
			}
		}
		else {
			std::cout << "Finite Element Datasets are not supported!" << std::endl;
			err = tecFileWriterClose(&szlFileHandle);
			//throw
		}
		
		if(outZoneIdx != zidx+1) {
			throw szlWriterError("zone index from TECIO [" + std::to_string(outZoneIdx) + "] does not match current zone loop index [" + std::to_string(zidx+1) + "]",1);
		}

		if(timeStrand) {
			double solnTime = dataContainer.zoneDetails[zidx].get_solutionTime();
			err = tecZoneSetUnsteadyOptions(szlFileHandle, outZoneIdx, solnTime, timeStrand);
			if(err) {
				throw szlWriterError("TECIO error creating unsteady options for zone " + std::to_string(zidx+1), 1);
			}
		}

		for(int v = 0; v < nVars; v++) {
			if(!(*sharingSource)[v] && !(*passiveVars)[v]) {
				//if not passive and not shared
				int nValues;
				if((*valueLocation)[v]) {
					//node value location
					nValues = dataContainer.zoneDetails[zidx].get_size();
				}
				else {
					//cell centered value location
					nValues = dataContainer.zoneDetails[zidx].get_size(false);
				}
				write_zoneData(zidx, v, nValues, dataContainer.vars[v][zidx]);
			}
		}
	}

	void szlWriter::write_zoneData(int zidx, int vidx, int nValues, zoneData &subzoneData) {
		int err;
		switch(subzoneData.type()) {
			case dataTypeFlag::singlePrecision:
				err = tecZoneVarWriteFloatValues(szlFileHandle, zidx+1, vidx+1, 0, 
						nValues, &subzoneData.get_float(0));
				break;
			
			case dataTypeFlag::doublePrecision:
				err = tecZoneVarWriteDoubleValues(szlFileHandle, zidx+1, vidx+1, 0, 
						nValues, &subzoneData.get_double(0));
				break;

			case dataTypeFlag::int32:
				err = tecZoneVarWriteInt32Values(szlFileHandle, zidx+1, vidx+1, 0, 
						nValues, &subzoneData.get_int32(0));
				break;

			case dataTypeFlag::int16:
				err = tecZoneVarWriteInt16Values(szlFileHandle, zidx+1, vidx+1, 0, 
						nValues, &subzoneData.get_int16(0));
				break;
			
			case dataTypeFlag::byte:
				err = tecZoneVarWriteUInt8Values(szlFileHandle, zidx+1, vidx+1, 0, 
						nValues, &subzoneData.get_byte(0));
				break;

			default:
				throw szlWriterError("unrecognized dataTypeFlag for writing to szl file at zone " + std::to_string(zidx+1), 2);
		}
		if(err) {
			throw szlWriterError("TECIO error for writing data for zone " + std::to_string(zidx+1),1);
		}
	}

	void szlWriter::write_file(fileContainer &dataContainer, bool verbose) {
		int err; //var to store error code
		try {
			int32_t outputDebugInfo = verbose;
			
			//get file header information
			std::string varlist;
			int nVars = dataContainer.vars.size();
			int nZones = dataContainer.zoneDetails.size();
			for(int v = 0; v < nVars; v++) {
				varlist += dataContainer.vars[v].get_name();	
				if(v != nVars-1) { varlist+=",";}
			}
			
			err = tecFileWriterOpen(fname.c_str(), dataContainer.title.c_str(), varlist.c_str(), 
					SZPLT_FORMAT, (int32_t)dataContainer.fType, 0, NULL, &szlFileHandle);

			if(err) {
				throw szlReaderError("TECIO error opening file \"" + fname + "\"", 1);
			}
			
			err = tecFileSetDiagnosticsLevel(szlFileHandle, outputDebugInfo);
			
			for(int z = 0; z < nZones; z++) {
				write_currentZone(z, dataContainer, nVars, err);
			}
		}

		catch(szlWriterError const &e) {
			std::cout << "\nSZL WRITING ERROR: " << e.what() << " (CODE: " << e.code+e.secondary_code << ")" << std::endl;
		}

		catch(containerError const &e) {
			std::cout << e.what() << std::endl;
		}

		err = tecFileWriterClose(&szlFileHandle);

	}
	
	void szlWriter::write_file(std::string _fname, fileContainer &dataContainer, bool verbose) {
		fname = _fname;
		write_file(dataContainer, verbose);
	}

}

