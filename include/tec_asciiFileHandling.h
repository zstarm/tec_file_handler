#ifndef TEC_ASCII_FILEHANDLE
#define TEC_ASCII_FILEHANDLE

#include "tec_asciiFormat.h"
#include "tec_error.h"
#include "tec_fileContainer.h"

#include <fstream>

namespace tec {
	class asciiReader { std::string fname;
		std::ifstream in_fs; 

		asciiFormatter line_formatter;

		int zoneCounter;
		int zoneSize;
		int dataCount;
		int varCount;

		void parse_headerLine(std::string &line, fileContainer &dataContainer);
		void parse_zoneLine(std::string &line, fileContainer &dataContainer);
		void preprocess_data(std::string &line, fileContainer &dataContainer);

		void parse_pointFormatData(std::string &line, fileContainer &dataContainer); 
		void parse_blockFormatData(std::string &line, fileContainer &dataContainer); 

		void parse_dataTypeList(size_t& pos_tmp, std::string& field, fileContainer &dataContainer);
		void parse_shareList(size_t& pos_tmp, std::string& field, fileContainer &dataContainer);
		void parse_passiveList(size_t& pos_tmp, std::string& field, fileContainer &dataContainer);
		

		public:
			asciiReader();
			asciiReader(std::string _fname);
			asciiReader(const char* _fname);
			asciiReader(const asciiReader &obj) = delete;
			asciiReader(asciiReader &&obj) = delete;
			~asciiReader();
			
			asciiReader& operator=(const asciiReader &obj) = delete;
			asciiReader& operator=(asciiReader &&obj) = delete;

			void read_file(fileContainer &dataContainer);
			void read_file(std::string _fname, fileContainer &dataContainer);
	};
	
	class asciiWriter {
		
		std::string fname;
		std::ofstream out_fs;

		void write_currentZone(int zidx, fileContainer &dataContainer, int &nVars, bool verbose);

		public:
			asciiWriter();
			asciiWriter(std::string _fname);
			asciiWriter(const char* _fname);
			asciiWriter(const asciiWriter &obj) = delete;
			asciiWriter(asciiWriter &&obj) = delete;
			~asciiWriter();

			asciiWriter& operator=(const asciiWriter &obj) = delete;
			asciiWriter& operator=(asciiWriter &&obj) = delete;

			void write_file(fileContainer &dataContainer, bool verbose = true);
			//void write_file(std::string _fname, fileContainer, &dataContainer, bool verbose = true);

	};
}
#endif
