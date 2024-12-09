#ifndef TEC_ERROR
#define TEC_ERROR
#include <string>

namespace tec {
	struct asciiReaderError {
		const std::string error_msg;
		asciiReaderError();
		asciiReaderError(const char* msg);
		asciiReaderError(std::string msg);
		const std::string& what() const noexcept;
	};

	struct containerError  {
		const std::string error_msg;
		containerError();
		containerError(const char* msg);
		containerError(std::string msg);
		const std::string& what() const noexcept;
	};
}
#endif 
