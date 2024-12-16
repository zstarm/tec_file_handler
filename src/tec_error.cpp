#include "tec_error.h"
namespace tec {
	tec_error::tec_error(std::string msg, int pcode, int scode) : error_msg(msg), code(pcode), secondary_code(scode) {}
	
	const std::string& tec_error::what() const noexcept {
		return error_msg; 
	}

	asciiReaderError::asciiReaderError() : tec_error("No message provided", 200, 0) {}

	asciiReaderError::asciiReaderError(const char* msg, int type) : tec_error(msg, 200, type) {}

	asciiReaderError::asciiReaderError(std::string msg, int type) : tec_error(msg, 200, type) {}


	containerError::containerError() : tec_error("No message provided", 100, 0) {}

	containerError::containerError(const char* msg, int type) : tec_error(msg, 100, type) {}

	containerError::containerError(std::string msg, int type) : tec_error(msg, 100, type) {}


	szlReaderError::szlReaderError() : tec_error("No message provided", 400, 0) {}

	szlReaderError::szlReaderError(const char* msg, int type) : tec_error(msg, 400, type) {}

	szlReaderError::szlReaderError(std::string msg, int type) : tec_error(msg, 400, type) {}

	szlWriterError::szlWriterError() : tec_error("No message provided", 500, 0) {}

	szlWriterError::szlWriterError(const char* msg, int type) : tec_error(msg, 500, type) {}

	szlWriterError::szlWriterError(std::string msg, int type) : tec_error(msg, 500, type) {}

}
