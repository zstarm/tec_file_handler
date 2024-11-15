#include "tec_asciiFormat.h"

tec_asciiFormatter::tec_asciiFormatter() {}

tec_asciiFormatter::~tec_asciiFormatter() {}

void tec_asciiFormatter::change_pattern(std::string && new_pattern) {
	pattern = std::regex(new_pattern, std::regex::icase); 
}

bool tec_asciiFormatter::format_header(std::string &line, const char* delim, const char* separator) {

	change_pattern("\\bTITLE\\b|\\bVARIABLES\\b|\\bFILETYPE\\b");
	if(std::regex_search(line, pattern)) {
		
		change_pattern("\\s*=+\\s*"); //remove any whitespace before and after the equal signs
		line = std::regex_replace(line, pattern, "=");

		change_pattern("\\s+(?=([^\"]*\"[^\"]*\")*[^\"]*$)"); //replace any white space not
															  //within quotes with a comma
		line = std::regex_replace(line, pattern, delim);
		
		change_pattern("^,|,(?=,+)|,$"); //remove any oddly placed comma
		line = std::regex_replace(line, pattern, ""); 

		change_pattern("\""); //safe to remove all commas now
		line = std::regex_replace(line, pattern, ""); 
		
		change_pattern(",(?=TITLE)|,(?=VARIABLES)|,(?=FILETYPE)"); //separate out header keywords
		line = std::regex_replace(line, pattern, separator);

		return true;
	}

	return false;
}

bool tec_asciiFormatter::format_subzoneHeader(std::string &line, const char* delim, const char* separator) {
	change_pattern("\\bZONE\\b|\\bZONETYPE\\b|\\bI\\b|\\bJ\\b|\\bK\\b|\\bDT\\b|\\bF\\b|"
			"\\bDATAPACKING\\b|\\bVARSHARELIST\\b|\\bCONNECTIVITYSHAREZONE\\b|\\STRANDID\\b|"
			"\\bSOLUTIONTIME\\b");
	//\\bNODES\\b|\\bELEMENTS\\b|\\bELEMENTS\\b|\\bFACES\\bTOTALNUMFACE");
	if(std::regex_search(line, pattern)) {
		
		change_pattern("\\s*=+\\s*"); //remove any whitespace before and after the equal signs
		line = std::regex_replace(line, pattern, "=");

		change_pattern("\\s+(?=([^\"]*\"[^\"]*\")*[^\"]*$)"); //replace any white space not
															  //within quotes with a comma
		line = std::regex_replace(line, pattern, delim);
		
		change_pattern("^,|,(?=,+)|,$"); //remove any oddly placed comma
		line = std::regex_replace(line, pattern, ""); 

		change_pattern("\""); //safe to remove all commas now
		line = std::regex_replace(line, pattern, ""); 
		
		return true;
	}

	return false;
}

int tec_asciiFormatter::format_auto(std::string &line, const char* delim, const char* separator) {
	
	//change_pattern("^\\s+"); //always remove any white space at beginning of lines
	//line = std::regex_replace(line, pattern, ""); 
	
	if(format_header(line, delim, separator)) {
		return 1;
	}
	else if(format_subzoneHeader(line, delim, separator)) {
		return 2;
	}
	
	return 0;

}
