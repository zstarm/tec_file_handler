#ifndef TEC_ERROR
#define TEC_ERROR
#include <string>

struct tec_asciiReaderError {
	const std::string error_msg;
	tec_asciiReaderError();
	tec_asciiReaderError(const char* msg);
	tec_asciiReaderError(std::string msg);
	const std::string& what() const noexcept;
};

struct tec_containerError  {
	const std::string error_msg;
	tec_containerError();
	tec_containerError(const char* msg);
	tec_containerError(std::string msg);
	const std::string& what() const noexcept;
};


#endif 
