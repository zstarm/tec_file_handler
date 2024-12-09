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
			"\\bDATAPACKING\\b|\\bVARSHARELIST\\b|\\bPASSIVEVARLIST\\b|\\STRANDID\\b|"
			"\\bSOLUTIONTIME\\b");

	//\\bNODES\\b|\\bELEMENTS\\b|\\bCONNECTIVITYSHAREZONE\\b|\\bELEMENTS\\b|\\bFACES\\bTOTALNUMFACE");
	if(std::regex_search(line, pattern)) {
		
		change_pattern("\\s*=+\\s*"); //remove any whitespace before and after the equal signs
		line = std::regex_replace(line, pattern, "=");
		change_pattern("\\s*-+\\s*"); //remove any whitespace before and after the minus signs
		line = std::regex_replace(line, pattern, "-");

		change_pattern("\\s+(?=([^\"]*\"[^\"]*\")*[^\"]*$)"); //replace any white space not
															  //within quotes with a comma
		line = std::regex_replace(line, pattern, delim);
		
		//remove any oddly placed comma
		change_pattern("^,|,+(?=,)|,+(?=\\))|,+(?=\\])|,$"); 
		line = std::regex_replace(line, pattern, ""); 
		change_pattern("\\(,+"); 
		line = std::regex_replace(line, pattern, "("); 
		change_pattern("\\[,+"); 
		line = std::regex_replace(line, pattern, "["); 

		change_pattern("\""); //safe to remove all quotes now
		line = std::regex_replace(line, pattern, ""); 
		
		return true;
	}

	return false;
}

bool tec_asciiFormatter::check_orphan_var(std::string &line, const char* delim) {
	std::string tmp;
	
	//replace any white space not within quotes with a comma
	change_pattern("\\s+(?=([^\"]*\"[^\"]*\")*[^\"]*$)"); 
	tmp = std::regex_replace(line, pattern, delim);

	//search for one or more variable name in quotes on line
	change_pattern("\"(\\s*\\w+\\s*)*\""); 
	if(std::regex_search(line, pattern)) {
		
		change_pattern("^,|,(?=,+)|,$"); //remove any oddly placed commas from previous step
		tmp = std::regex_replace(tmp, pattern, ""); 
		
		line = "VARIABLES=" + tmp; //modifiy line to have "VARIABLES" keyword identifier 
		
		change_pattern("\""); //safe to remove all commas now
		line = std::regex_replace(line, pattern, ""); 
		return true;
	}

	return false;
}

bool tec_asciiFormatter::check_invalidLine(std::string &line) {
	change_pattern("[a-df-zA-DF-Z]");
	if(std::regex_search(line, pattern)) {
		return true;
	}
	return false;
}

int tec_asciiFormatter::format_auto(std::string &line, const char* delim, const char* separator) {
	
	if(format_header(line, delim, separator)) {
		return 1; //return header classification
	}
	else if(format_subzoneHeader(line, delim, separator)) {
		return 2; //return subzone header classification
	}
	else if(check_orphan_var(line)) {
		return 1; //return header classification
	}
	else if(check_invalidLine(line)) {
		return -1; //return invalid line classification
	}

	change_pattern("\\t+"); //change any tabs to single spaces
	line = std::regex_replace(line, pattern, " "); 
	change_pattern("^\\s+|\\s(?=\\s+)|\\s$"); //remove any double+ spaces and spaces at the start/end
	line = std::regex_replace(line, pattern, ""); 
	return 0; //return data line classification
}
