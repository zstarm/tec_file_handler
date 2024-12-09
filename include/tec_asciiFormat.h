#ifndef TEC_ASCII_FORMAT
#define TEC_ASCII_FORMAT

#include <regex>
#include <iostream>
#include <string>

namespace tec {
	class asciiFormatter {
		private:
			std::regex pattern;

			void change_pattern(std::string && new_pattern);

		public:
			asciiFormatter();
			~asciiFormatter();

			bool format_header(std::string &line, const char* delim = ",", const char* separator = "\n");
			bool format_subzoneHeader(std::string &line, const char* delim = ",", const char* separator = "\n");

			bool check_orphan_var(std::string &line, const char* delim = ",");
			bool check_invalidLine(std::string &line);

			int format_auto(std::string &line, const char* delim = ",", const char* separator = "\n");

	};
}
#endif 
