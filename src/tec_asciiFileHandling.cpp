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
			dataContainer.zoneDetails.push_back(zoneCounter); //push back new instance of zone details
			dataCount = 0; //reset the data counter for the zone
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

void tec_asciiReader::preprocess_data(std::string &line, tec_fileContent &dataContainer) {
	if(!zoneCounter) {
		//if no zone header was encountered, create a single zone
		zoneCounter++; 
		dataContainer.zoneDetails.push_back(1); //create single instance of zone details
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
				throw tec_asciiReaderError("BLOCK formatting was specified but zone size (I, J, or K) was not specified!");
			}
			parse_blockFormatData(line, dataContainer);
		}
	}

	else {
		//FINITE ELEMENT DATASET
		throw tec_asciiReaderError("reader currently doesn't not support finte element datasets");
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
			if(dataContainer.variables[v].subzoneData.size() != zoneCounter) {
				//resize subzoneData vector to match zone counter of reader
				//should only occur if the data type for zone was not specified
				dataContainer.variables[v].subzoneData.resize(zoneCounter);
			}
			type = dataContainer.variables[v][zoneCounter-1].type(); //get data type of current variable
			//extract single data entry in the file
			pos = line.find(' ');
			if(pos == std::string::npos) {
				if(v+1 != nVars) {
					throw tec_asciiReaderError("Not enough columns for identified non-shared variables");
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
						throw tec_asciiReaderError("datatype identifier unsupported and could not be handled");
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
						throw tec_asciiReaderError("datatype identifier unsupported and could not be handled");
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
						throw tec_asciiReaderError("datatype identifier unsupported and could not be handled");
				}
			}
		}
	}
	else {
		//number of variables is unknown
		bool repeat = true;
		while(repeat) {
			//push back a variable with default settings
			dataContainer.variables.push_back("V" + std::to_string(++nVars));
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
		}
	}
}

void tec_asciiReader::parse_blockFormatData(std::string &line, tec_fileContent &dataContainer) {
	if(dataCount == zoneSize) {
		//if the total number of data points has been read for the variable
		//reset dataCount and move on to reading the next variable
		varCount++;
		dataCount = 0;
	}
	if(dataContainer.variables[varCount].subzoneData.size() != zoneCounter) {
		//resize subzoneData vector to match zone counter of reader
		//should only occur if the data type for zone was not specified
		dataContainer.variables[varCount].subzoneData.resize(zoneCounter);
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
				throw tec_asciiReaderError("datatype identifier unsupported and could not be handled");
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
				throw tec_asciiReaderError("datatype identifier unsupported and could not be handled");
		}
		dataCount++;
	}
}

void tec_asciiReader::read_file(tec_fileContent &dataContainer) {
	try {
		if(!fname.empty()) {
			in_fs = std::ifstream(fname);

			if(in_fs.is_open()) {
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
			}

			else {
				std::cout << "ERROR!: could not open or find file name: " << fname << std::endl;
			}
		}

		else {
			std::cout << "ERROR!: file name is empty" << std::endl;
		}

	}

	catch(tec_asciiReaderError const &e) {
		std::cout << "ASCII READER ERROR!: " << e.what() << std::endl;
	}
	
	catch(tec_containerError const &e) {
		std::cout << "tec_fileContent ERROR: " << e.what() << std::endl;
	}

}

void tec_asciiReader::read_file(std::string _fname, tec_fileContent &dataContainer) {
	//change the file name can call general reader function
	fname = _fname;
	read_file(dataContainer);
}
