#include "tec_error.h"
namespace tec {
	tec_error::tec_error(std::string msg, int pcode, int scode) : error_msg(msg), code(pcode), secondary_code(scode) {}
	/*	
	const std::string& tec_error::what() const noexcept {
		return error_msg; 
	}
	*/

	std::string tec_error::what() {
		std::string out = error_msg + " (code = " + std::to_string(code+secondary_code) + ")";
		return out;
	}
	
	//-----------------------------------------------------------------------------------------
	// ASCII FILE HANDLING ERRORS
	//-----------------------------------------------------------------------------------------
	
	asciiFileError::asciiFileError() : tec_error("ASCII file handling error", 200, 0) {}

	asciiFileError::asciiFileError(const char* msg, int type) : tec_error(msg, 200, type) {}
	
	asciiFileError::asciiFileError(std::string msg, int type) : tec_error(msg, 200, type) {}

	
	asciiReaderError::asciiReaderError() : asciiFileError("ASCII file reading error", 10) {}

	asciiReaderError::asciiReaderError(const char* msg, int type) : asciiFileError(msg, 10+type) {}

	asciiReaderError::asciiReaderError(std::string msg, int type) : asciiFileError(msg, 10+type) {}
	
	//-----------------------------------------------------------------------------------------
	// SZL FILE HANDLING ERRORS
	//-----------------------------------------------------------------------------------------
	
	szlFileError::szlFileError() : tec_error("SZL file handling error", 400, 0) {}

	szlFileError::szlFileError(const char* msg, int type) : tec_error(msg, 400, type) {}
	
	szlFileError::szlFileError(std::string msg, int type) : tec_error(msg, 400, type) {}

	
	szlReaderError::szlReaderError() : szlFileError("SZL file reading error", 10) {}

	szlReaderError::szlReaderError(const char* msg, int type) : szlFileError(msg, 10+type) {}

	szlReaderError::szlReaderError(std::string msg, int type) : szlFileError(msg,10+type) {}

	
	szlWriterError::szlWriterError() : tec_error("SZL file writing error", 20) {}

	szlWriterError::szlWriterError(const char* msg, int type) : szlFileError(msg, 20+type) {}

	szlWriterError::szlWriterError(std::string msg, int type) : szlFileError(msg, 20+type) {}

	//-----------------------------------------------------------------------------------------
	// FILE CONTAINER ERRORS
	//-----------------------------------------------------------------------------------------

	containerError::containerError() : tec_error("file container error", 100, 0) {}

	containerError::containerError(const char* msg, int type) : tec_error(msg, 100, type) {}

	containerError::containerError(std::string msg, int type) : tec_error(msg, 100, type) {}

	
	variableError::variableError() : containerError("tec::variable error", 10) {}
	
	variableError::variableError(const char* msg, int type) : containerError(msg, 10+type) {}

	variableError::variableError(std::string msg, int type) : containerError(msg, 10+type) {}

	zoneDataError::zoneDataError() : containerError("tec::zoneData error", 20) {}
	
	zoneDataError::zoneDataError(const char* msg, int type) : containerError(msg, 20+type) {}

	zoneDataError::zoneDataError(std::string msg, int type) : containerError(msg, 20+type) {}

	zoneInfoError::zoneInfoError() : containerError("tec::zoneInfomation error", 30) {}
	
	zoneInfoError::zoneInfoError(const char* msg, int type) : containerError(msg, 30+type) {}

	zoneInfoError::zoneInfoError(std::string msg, int type) : containerError(msg, 30+type) {}

}
