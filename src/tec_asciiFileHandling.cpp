#include "tec_asciiFileHandling.h"

tec_asciiReader::tec_asciiReader(std::string _fname) : zoneCounter(0) {
	fname = _fname;
}

tec_asciiReader::tec_asciiReader(const char* _fname) : zoneCounter(0) {
	fname = std::string(_fname);
}

tec_asciiReader::~tec_asciiReader() {}

void tec_asciiReader::parse_headerLine(std::string &line, tec_fileContent &dataContainer) {
	size_t pos; 
	std::string field;
	while(!line.empty()) {
		pos = line.find('\n');
		if(pos == std::string::npos) {
			pos = line.length();
		}
		field = line.substr(0, pos);
		switch (std::toupper(field[0])) {
			case 'F':
			{
				switch(std::toupper(field[9])) {
					case 'F':
					{
						dataContainer.fType = fileTypeFlag::full;
						break;
					}
					case 'G':
					{
						dataContainer.fType = fileTypeFlag::grid;
						break;
					}
					case 'S':
					{
						dataContainer.fType = fileTypeFlag::solution;
						break;
					}
					default:
					{
						std::cout << "WARNING!: Unexpected file type identifier... Defaulting to FULL" << std::endl;
						dataContainer.fType = fileTypeFlag::full;
					}
				}
				break;
			}
			case 'T': 
			{
				dataContainer.title = field.substr(6); //capture what is after "TITLE" keyword
				break;
			}

			case 'V':
			{
				//remove VARIABLES keyword then parse through the variable list
				field.erase(0,10);
				//int count = 0;
				size_t pos_tmp;
				while((pos_tmp = field.find(',')) != std::string::npos) {
					dataContainer.variables.push_back(field.substr(0, pos_tmp));
					field.erase(0,++pos_tmp);
				}
				dataContainer.variables.push_back(field); //capture the last variable in the list
				break;
			}

			default:
			{
				std::cout << "WARNING!: ignoring an unexpected keyword identifier found" << std::endl;
			}
		}
		line.erase(0,++pos); 
	}
}

void tec_asciiReader::parse_zoneLine(std::string &line, tec_fileContent &dataContainer) {
	size_t pos; 
	std::string field;
	while(!line.empty()) {
		pos = line.find(',');
		if(pos == std::string::npos) {
			pos = line.length();
		}
		field = line.substr(0,pos);
		char ID = std::toupper(field[0]);
		size_t pos_tmp = field.find('=');
		if(pos_tmp == std::string::npos && ID == 'Z') {
			//creates new subzone if ZONE keyword is encountered
			zoneCounter++; //increase counter
			//push back new instance of zone details
			dataContainer.zoneDetails.push_back({zoneCounter, dataContainer.variables.size()}); 
			//reset the data counter and variable counter for the zone
			dataCount = 0; 
			varCount = 0;
			line.erase(0,pos+1);
			continue;
		}

		switch(ID) {
			case 'D':
			{
				char tmp_char = std::toupper(field[1]);
				if(tmp_char == 'A') {
					//DATAPACKING TYPE
					dataContainer.zoneDetails[zoneCounter-1].set_formatType(field[++pos_tmp]);
				}
				else if(tmp_char == 'T') {
					//parse for datatype of variables
					//first need to redo field substr to capture full DT list
					pos = line.find(')');
					field = line.substr(pos_tmp+2,pos-pos_tmp); 
					pos++; //increment pos s.t. it is the position of the comma after the parens
						   //ensures reader moves to the next keyword for next loop/call
					
					//now parse throw the list of variable DTs
					parse_dataTypeList(pos_tmp, line, field, dataContainer);	
				}
				else {
					std::cout << "WARNING!: ignoring an unrecognized/unsupported subzone keyword identifier" << std::endl;
				}
				break;
			}
			case 'F':
			{
				//ALIAS FOR DATAPACKING FORMAT
				dataContainer.zoneDetails[zoneCounter-1].set_formatType(field[++pos_tmp]);
				break;
			}
			case 'I':
			{
				dataContainer.zoneDetails[zoneCounter-1].set_IJKSize('I', std::stoi(field.substr(++pos_tmp)));
				break;

			}
			case 'J':
			{
				dataContainer.zoneDetails[zoneCounter-1].set_IJKSize('J', std::stoi(field.substr(++pos_tmp)));
				break;

			}
			case 'K':
			{
				dataContainer.zoneDetails[zoneCounter-1].set_IJKSize('K', std::stoi(field.substr(++pos_tmp)));

				break;
			}
			case 'S':
			{
				char tmp_char = std::toupper(field[1]);
				if(tmp_char == 'O') {
					//SOLUTION TIME
					dataContainer.zoneDetails[zoneCounter-1].set_solutionTime(std::stod(field.substr(++pos_tmp)));

				}
				else if(tmp_char == 'T') {
					//STRAND ID 
					dataContainer.zoneDetails[zoneCounter-1].set_strandID(std::stoi(field.substr(++pos_tmp)));
				}
				else {
					std::cout << "WARNING!: ignoring an unrecognized/unsupported subzone keyword identifier" << std::endl;
				}
				break;
			}
			case 'T':
			{
				dataContainer.zoneDetails[zoneCounter-1].set_zoneTitle(field.substr(++pos_tmp));
				break;
			}
			case 'Z':
			{
				dataContainer.zoneDetails[zoneCounter-1].set_zoneType(field[++pos_tmp]);
				break;
			}
			default:
					std::cout << "WARNING!: ignoring an unrecognized/unsupported subzone keyword identifier" << std::endl;

		}
		line.erase(0,++pos);
	}	
}

void tec_asciiReader::parse_dataTypeList(size_t& pos_tmp, std::string& line, std::string& field, tec_fileContent &dataContainer) {
	std::cout << "data type list parser called for zone " << zoneCounter << std::endl;
	varCount = 0;
	char tmp_char;
	bool keep_parsing = true;
	int nVars = dataContainer.variables.size();
	while(keep_parsing) {
		pos_tmp = field.find(',');
		if(pos_tmp == std::string::npos) {
			keep_parsing = false;
			pos_tmp = field.length();
		}
		
		bool resize_needed = false;
		if(!nVars) {
			//if the number of variables is unknown push back instances of vars
			dataContainer.variables.push_back("V"+std::to_string(varCount+1));
			resize_needed = true;
		}
		tmp_char = std::toupper(field[0]);
		if(tmp_char == 'D') {
			dataContainer.zoneDetails[zoneCounter-1].set_varDT(varCount, dataTypeFlag::doublePrecision, resize_needed);
			while(dataContainer.variables[varCount].subzoneData.size() < zoneCounter) {
				dataContainer.variables[varCount].subzoneData.push_back(dataTypeFlag::doublePrecision);
			}
		}
		else if(tmp_char == 'L') {
			dataContainer.zoneDetails[zoneCounter-1].set_varDT(varCount, dataTypeFlag::int32);
			while(dataContainer.variables[varCount].subzoneData.size() < zoneCounter) {
				dataContainer.variables[varCount].subzoneData.push_back(dataTypeFlag::int32);
			}	
		}
		else if(tmp_char == 'S') {
			tmp_char = std::toupper(field[1]);
			if(tmp_char == 'H') {
				dataContainer.zoneDetails[zoneCounter-1].set_varDT(varCount, dataTypeFlag::int16, resize_needed);
				while(dataContainer.variables[varCount].subzoneData.size() < zoneCounter) {
					dataContainer.variables[varCount].subzoneData.push_back(dataTypeFlag::int16);
				}
			}
			else {
				dataContainer.zoneDetails[zoneCounter-1].set_varDT(varCount, dataTypeFlag::singlePrecision, resize_needed);
				while(dataContainer.variables[varCount].subzoneData.size() < zoneCounter) {
					dataContainer.variables[varCount].subzoneData.push_back(dataTypeFlag::singlePrecision);
				}
			}
		}
		else if(tmp_char == 'B') {
			dataContainer.zoneDetails[zoneCounter-1].set_varDT(varCount, dataTypeFlag::byte, resize_needed);
			while(dataContainer.variables[varCount].subzoneData.size() < zoneCounter) {
				dataContainer.variables[varCount].subzoneData.push_back(dataTypeFlag::byte);
			}
		}
		else {
			std::cout << "WARNING!: unrecognized datatype... Defaulting to type SINGLE" << std::endl;
			dataContainer.zoneDetails[zoneCounter-1].set_varDT(varCount, dataTypeFlag::singlePrecision, resize_needed);
			while(dataContainer.variables[varCount].subzoneData.size() < zoneCounter) {
				dataContainer.variables[varCount].subzoneData.push_back(dataTypeFlag::singlePrecision);
			}
		}
		field.erase(0,++pos_tmp); //erase through the next inner list comma
		varCount++; //increment varCount while searching
	}
	varCount = 0; //reset the count for the data parsing functions
}

void tec_asciiReader::preprocess_data(std::string &line, tec_fileContent &dataContainer) {
	if(!zoneCounter) {
		//if no zone header was encountered, create a single zone
		zoneCounter++; 
		dataContainer.zoneDetails.push_back({1, dataContainer.variables.size()}); //create single instance of zone details
		dataContainer.zoneDetails[zoneCounter].set_zoneTitle("ZONE 001");
		dataCount = 0;
		varCount = 0;
	}
	if(!dataCount) {
		zoneSize = dataContainer.zoneDetails[zoneCounter-1].get_size(); 
	}

	if(dataContainer.zoneDetails[zoneCounter-1].get_zoneType() == zoneTypeFlag::ordered) { 
		//ORDERED DATASET
		if(dataContainer.zoneDetails[zoneCounter-1].get_formattingType() == formattingFlag::point) {
			//POINT formatting
			parse_pointFormatData(line, dataContainer);
			dataCount++;
		}
		else {
			//BLOCK formatting
			//check to make sure zone size is known
			if(!zoneSize) {
				throw tec_asciiReaderError("BLOCK formatting specified for zone " + std::to_string(zoneCounter) + " but no dimensions (I, J, or K) were found!");
			}
			if(!dataContainer.variables.size()) {
				throw tec_asciiReaderError("BLOCK formatting specified for zone " + std::to_string(zoneCounter) + " but number of variables is unknown");
			}
			parse_blockFormatData(line, dataContainer);
		}
	}

	else {
		//FINITE ELEMENT DATASET
		throw tec_asciiReaderError("ASCII reader currently doesn't not support finte element datasets");
		dataCount++;
	}

}

void tec_asciiReader::parse_pointFormatData(std::string &line, tec_fileContent  &dataContainer) {
	int nVars = dataContainer.variables.size();
	dataTypeFlag type;
	std::string entry;
	size_t pos;
	if(nVars) {
		//if variables are known, iterate through the stored tec_variables and enter data
		for(int v = 0; v < nVars; v++) {
			while(dataContainer.variables[v].subzoneData.size() < zoneCounter) {
				//push back subzoneData vector to match zone counter of reader
				//should only occur if the data type for zone was not specified
				dataContainer.variables[v].subzoneData.push_back(dataTypeFlag::singlePrecision);
			}
			type = dataContainer.variables[v][zoneCounter-1].type(); //get data type of current variable
			//extract single data entry in the file
			pos = line.find(' ');
			if(pos == std::string::npos) {
				if(v+1 != nVars) {
					throw tec_asciiReaderError("ASCII reader did not find enough columns for identified non-shared variables");
				}
				else {
					entry = line;
				}
			}
			else {
				entry = line.substr(0, pos);
				line.erase(0,pos+1);
			}
			if(!dataCount && zoneSize) {
				//if a fresh subzone of known size, check datatype allocate needed space 
				switch(type) {
					case dataTypeFlag::singlePrecision:
						dataContainer.variables[v].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::singlePrecision);
						break;
					
					case dataTypeFlag::doublePrecision:
						dataContainer.variables[v].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::doublePrecision);
						break;

					case dataTypeFlag::int32:
						dataContainer.variables[v].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::int32);
						break;

					case dataTypeFlag::int16:
						dataContainer.variables[v].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::int16);
						break;

					case dataTypeFlag::byte:
						dataContainer.variables[v].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::byte);
						break;

					default:
						throw tec_asciiReaderError("datatype identifier unsupported and could not be handled by the ASCII reader");
				}
			}
			if(zoneSize) {
				//if size is known, insert data point
				switch(type) {
					case dataTypeFlag::singlePrecision:
						dataContainer.variables[v][zoneCounter-1].get_float(dataCount) = std::stof(entry);
						break;
					
					case dataTypeFlag::doublePrecision:
						dataContainer.variables[v][zoneCounter-1].get_double(dataCount) = std::stod(entry);
						break;

					case dataTypeFlag::int32:
						dataContainer.variables[v][zoneCounter-1].get_int32(dataCount) = std::stol(entry);
						break;

					case dataTypeFlag::int16:
						dataContainer.variables[v][zoneCounter-1].get_int16(dataCount) = (int16_t)std::stoi(entry);
						break;

					case dataTypeFlag::byte:
						dataContainer.variables[v][zoneCounter-1].get_byte(dataCount) = (uint8_t)std::stoi(entry);
						break;

					default:
						throw tec_asciiReaderError("datatype identifier unsupported and could not be handled by the ASCII reader");
				}
			}

			else {
				//use push_back if size is not provided
				switch(type) {
					case dataTypeFlag::singlePrecision:
						dataContainer.variables[v][zoneCounter-1].push_back(std::stof(entry));
						break;
					
					case dataTypeFlag::doublePrecision:
						dataContainer.variables[v][zoneCounter-1].push_back(std::stod(entry));
						break;

					case dataTypeFlag::int32:
						dataContainer.variables[v][zoneCounter-1].push_back((int32_t)std::stol(entry));
						break;

					case dataTypeFlag::int16:
						dataContainer.variables[v][zoneCounter-1].push_back((int16_t)std::stoi(entry));
						break;

					case dataTypeFlag::byte:
						dataContainer.variables[v][zoneCounter-1].push_back((uint8_t)std::stoi(entry));
						break;

					default:
						throw tec_asciiReaderError("datatype identifier unsupported and could not be handled by the ASCII reader");
				}
			}
		}
	}
	else {
		//number of variables is unknown
		bool repeat = true;
		varCount = 0;
		while(repeat) {
			//push back a variable with default settings
			dataContainer.variables.push_back("V" + std::to_string(++nVars));
			//std::cout << "called " << nVars << " times" << std::endl;
			dataContainer.zoneDetails[zoneCounter-1].set_varDT(varCount, dataTypeFlag::singlePrecision, true);
			pos = line.find(' ');
			if(pos == std::string::npos) {
				repeat = false;
				entry = line;
				line.erase(0);
			}
			else {
				entry = line.substr(0, pos);
				line.erase(0,pos+1);
			}
			//no type checking is required as default type is "float/single" 
			if(!dataCount && zoneSize) {
				dataContainer.variables[nVars-1].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::singlePrecision);
			}
			if(zoneSize) {
				dataContainer.variables[nVars-1][zoneCounter-1].get_float(dataCount) = std::stof(entry);
			}
			else {
				dataContainer.variables[nVars-1][zoneCounter-1].push_back(std::stof(entry));
			}
			varCount++;
		}
		varCount = 0;
	}
}

void tec_asciiReader::parse_blockFormatData(std::string &line, tec_fileContent &dataContainer) {
	if(dataCount == zoneSize) {
		//if the total number of data points has been read for the variable
		//reset dataCount and move on to reading the next variable
		varCount++;
		dataCount = 0;
	}
	while(dataContainer.variables[varCount].subzoneData.size() < zoneCounter) {
		//push back a subzoneData instance to match zone counter of reader
		//should only occur if the data type for zone was not specified
		dataContainer.variables[varCount].subzoneData.push_back(dataTypeFlag::singlePrecision);
	}
	dataTypeFlag type;
	type = dataContainer.variables[varCount][zoneCounter-1].type(); //get data type of current variable
	
	if(!dataCount) {
		//if at the start of reading new variable, allocate space for desired datatype
		switch(type) {
			case dataTypeFlag::singlePrecision:
				dataContainer.variables[varCount].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::singlePrecision);
				break;
			
			case dataTypeFlag::doublePrecision:
				dataContainer.variables[varCount].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::doublePrecision);
				break;

			case dataTypeFlag::int32:
				dataContainer.variables[varCount].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::int32);
				break;

			case dataTypeFlag::int16:
				dataContainer.variables[varCount].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::int16);
				break;

			case dataTypeFlag::byte:
				dataContainer.variables[varCount].resize_zone(zoneCounter-1, zoneSize, dataTypeFlag::byte);
				break;

			default:
				throw tec_asciiReaderError("datatype identifier unsupported and could not be handled by the ASCII reader");
		}
	}
	
	std::string entry;
	size_t pos;
	bool repeat = true;
	while(repeat) {
		pos = line.find(' ');
		if(pos == std::string::npos) {
			/*
			if(dataCount+1 != zoneSize) {
				throw tec_asciiReaderError("Not enough point for specified size");
			}
			*/
			repeat = false;
			entry = line;
			line.erase(0);
		}
		else {
			entry = line.substr(0, pos);
			line.erase(0,pos+1);
		}
		switch(type) {
			case dataTypeFlag::singlePrecision:
				dataContainer.variables[varCount][zoneCounter-1].get_float(dataCount) = std::stof(entry);
				break;
			
			case dataTypeFlag::doublePrecision:
				dataContainer.variables[varCount][zoneCounter-1].get_double(dataCount) = std::stod(entry);
				break;

			case dataTypeFlag::int32:
				dataContainer.variables[varCount][zoneCounter-1].get_int32(dataCount) = std::stol(entry);
				break;

			case dataTypeFlag::int16:
				dataContainer.variables[varCount][zoneCounter-1].get_int16(dataCount) = (int16_t)std::stoi(entry);
				break;

			case dataTypeFlag::byte:
				dataContainer.variables[varCount][zoneCounter-1].get_byte(dataCount) = (uint8_t)std::stoi(entry);
				break;

			default:
				throw tec_asciiReaderError("datatype identifier unsupported and could not be handled by the ASCII reader");
		}
		dataCount++;
	}
}

void tec_asciiReader::read_file(tec_fileContent &dataContainer) {
	if(!fname.empty()) {
		in_fs = std::ifstream(fname);
		if(in_fs.is_open()) {
			try {
				std::string file_line;
				int lineCounter = 0;
				while(!in_fs.eof()) {
					std::getline(in_fs, file_line);
					if(!file_line.empty()) {
						int lineType = line_formatter.format_auto(file_line);
						switch(lineType) {
							case 0:
								preprocess_data(file_line, dataContainer);
								break;
							case 1:
								parse_headerLine(file_line, dataContainer);
								break;
							case 2:
								parse_zoneLine(file_line, dataContainer);
								break;
							default:
								std::cout << "WARNIING!: a line with invalid formating was encountered on line number: " << lineCounter+1 << std::endl;
						}
					}
					lineCounter++;
				}
				in_fs.close();
				for(int z = 0; z < dataContainer.zoneDetails.size(); z++) {
					if(!dataContainer.zoneDetails[z].get_size()) {
						int s = 0;
						int v = 0; 
						while(!s && v < dataContainer.variables.size()) {
							s = dataContainer.variables[v][z].get_array_size();
						}
						dataContainer.zoneDetails[z].set_IJKSize('I', s);
					}
				}
			}

			catch(tec_asciiReaderError const &e) {
				std::cout << "ASCII READER ERROR!: " << e.what() << std::endl;
				std::cout << "Exciting file reading..." << std::endl;
			}
			
			catch(tec_containerError const &e) {
				std::cout << "DATA CONTAINER ERROR: " << e.what() << std::endl;
				std::cout << "Exciting file reading..." << std::endl;
			}

		}

		else {
			std::cout << "ERROR!: could not open or find file name: " << fname << std::endl;
		}
	}

	else {
		std::cout << "ERROR!: file name is empty" << std::endl;
	}


}

void tec_asciiReader::read_file(std::string _fname, tec_fileContent &dataContainer) {
	//change the file name can call general reader function
	fname = _fname;
	read_file(dataContainer);
}
