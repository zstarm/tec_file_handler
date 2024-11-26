#include "tec_exceptions.h"

tec_readerException::exit_on_error() : error_msg("No message provided") {}

tec_readerException::exit_on_error(const char* msg) : error_msg(msg) {}

const char* tec_readerException::what() const noexcept {
	return error_msg;
}
