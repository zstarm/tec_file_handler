#include "tec_error.h"

tec_asciiReaderError::tec_asciiReaderError() : error_msg("No message provided") {}

tec_asciiReaderError::tec_asciiReaderError(const char* msg) : error_msg(msg) {}

const char* tec_asciiReaderError::what() const noexcept {
	return error_msg;
}

tec_containerError::tec_containerError() : error_msg("No message provided") {}

tec_containerError::tec_containerError(const char* msg) : error_msg(msg) {}

const char* tec_containerError::what() const noexcept {
	return error_msg;
}
