#ifndef TEC_ASCII_FILEHANDLE
#define TEC_ASCII_FILEHANDLE

#include "tec_asciiFormat.h"
#include "tec_error.h"
#include "tec_fileContent.h"

#include <fstream>

namespace tec {
	class asciiReader {
		std::string fname;
		std::ifstream in_fs; 

		asciiFormatter line_formatter;

		int zoneCounter;
		int zoneSize;
		int dataCount;
		int varCount;

		void parse_headerLine(std::string &line, fileContent &dataContainer);
		void parse_zoneLine(std::string &line, fileContent &dataContainer);
		void preprocess_data(std::string &line, fileContent &dataContainer);

		void parse_pointFormatData(std::string &line, fileContent &dataContainer); 
		void parse_blockFormatData(std::string &line, fileContent &dataContainer); 

		void parse_dataTypeList(size_t& pos_tmp, std::string& field, fileContent &dataContainer);
		void parse_shareList(size_t& pos_tmp, std::string& field, fileContent &dataContainer);
		void parse_passiveList(size_t& pos_tmp, std::string& field, fileContent &dataContainer);
		

		public:
			asciiReader();
			asciiReader(std::string _fname);
			asciiReader(const char* _fname);
			~asciiReader();

			void read_file(fileContent &dataContainer);
			void read_file(std::string _fname, fileContent &dataContainer);
	};
}
#endif
