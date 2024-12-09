#include "tec_error.h"
namespace tec {
	asciiReaderError::asciiReaderError() : error_msg("No message provided") {}

	asciiReaderError::asciiReaderError(const char* msg) : error_msg(msg) {}

	asciiReaderError::asciiReaderError(std::string msg) : error_msg(msg) {}

	const std::string& asciiReaderError::what() const noexcept {
		return error_msg;
	}

	containerError::containerError() : error_msg("No message provided") {}

	containerError::containerError(const char* msg) : error_msg(msg) {}

	containerError::containerError(std::string msg) : error_msg(msg) {}

	const std::string& containerError::what() const noexcept {
		return error_msg;
	}
}
