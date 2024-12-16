#ifndef TEC_ERROR
#define TEC_ERROR
#include <string>

namespace tec {
	struct tec_error {
		const int code;
		const int secondary_code;

		const std::string error_msg;
		tec_error(std::string msg, int pcode, int scode);
		const std::string& what() const noexcept;
	};

	struct asciiReaderError : public tec_error {

		asciiReaderError();
		asciiReaderError(const char* msg, int type = 0);
		asciiReaderError(std::string msg, int type = 0);
	};

	//struct asciiWriterError {};

	struct szlReaderError : public tec_error {
		szlReaderError();
		szlReaderError(const char* msg, int type = 0);
		szlReaderError(std::string msg, int type = 0);
	};

	struct szlWriterError : public tec_error {
		szlWriterError();
		szlWriterError(const char* msg, int type = 0);
		szlWriterError(std::string msg, int type = 0);
	};

	struct containerError : public tec_error {
		containerError();
		containerError(const char* msg, int type = 0);
		containerError(std::string msg, int type = 0);
	};

}
#endif 
