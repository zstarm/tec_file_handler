#ifndef TEC_ASCII_FILEHANDLE
#define TEC_ASCII_FILEHANDLE

#include "tec_asciiFormat.h"
#include "tec_error.h"
#include "tec_fileContent.h"

#include <fstream>

//#include <unordered_map>

class tec_asciiReader {
	std::string fname;
	std::ifstream in_fs; 

	tec_asciiFormatter line_formatter;

	int zoneCounter;
	int zoneSize;
	int dataCount;
	int varCount;

	void parse_headerLine(std::string &line, tec_fileContent &dataContainer);
	void parse_zoneLine(std::string &line, tec_fileContent &dataContainer);
	void preprocess_data(std::string &line, tec_fileContent &dataContainer);

	void parse_pointFormatData(std::string &line, tec_fileContent &dataContainer); 
	void parse_blockFormatData(std::string &line, tec_fileContent &dataContainer); 
	

	public:
		tec_asciiReader();
		tec_asciiReader(std::string _fname);
		tec_asciiReader(const char* _fname);
		~tec_asciiReader();

		void read_file(tec_fileContent &dataContainer);
		void read_file(std::string _fname, tec_fileContent &dataContainer);

};

#endif
