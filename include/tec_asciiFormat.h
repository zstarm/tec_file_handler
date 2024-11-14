#ifndef TEC_ASCII_FORMAT
#define TEC_ASCII_FORMAT

#include <regex>
#include <iostream>
#include <string>

class tec_asciiFormatter {
	private:
		std::regex pattern;

		void change_pattern(std::string && new_pattern);

	public:
		tec_asciiFormatter();
		~tec_asciiFormatter();

		bool format_header(std::string &line, const char* delim = ",", const char* separator = "\n");
		bool format_subzoneHeader(std::string &line, const char* delim = ",", const char* separator = "\n");

		int format_auto(std::string &line, const char* delim = ",", const char* separator = "\n");

};


#endif 
