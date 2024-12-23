#ifndef TEC_ERROR
#define TEC_ERROR
#include <string>

namespace tec {
	struct tec_error {
		const int code;
		const int secondary_code;

		const std::string error_msg;
		tec_error(std::string msg, int pcode, int scode);
		//const std::string& what() const noexcept;
		std::string what() const noexcept;
	};

	struct asciiFileError : public tec_error {
		asciiFileError();
		asciiFileError(const char* msg, int type = 0);
		asciiFileError(std::string msg, int type = 0);

	};

	struct asciiReaderError : public asciiFileError {
		asciiReaderError();
		asciiReaderError(const char* msg, int type = 0);
		asciiReaderError(std::string msg, int type = 0);
	};

	//struct asciiWriterError {};
	
	struct szlFileError : public tec_error {
		szlFileError();
		szlFileError(const char* msg, int type = 0);
		szlFileError(std::string msg, int type = 0);
	};

	struct szlReaderError : public szlFileError {
		szlReaderError();
		szlReaderError(const char* msg, int type = 0);
		szlReaderError(std::string msg, int type = 0);
	};

	struct szlWriterError : public szlFileError {
		szlWriterError();
		szlWriterError(const char* msg, int type = 0);
		szlWriterError(std::string msg, int type = 0);
	};

	struct containerError : public tec_error {
		containerError();
		containerError(const char* msg, int type = 0);
		containerError(std::string msg, int type = 0);
	};

	struct variableError : public containerError {
		variableError();
		variableError(const char* msg, int type = 0);
		variableError(std::string msg, int type = 0);
	};
	
	struct zoneDataError : public containerError {
		zoneDataError();
		zoneDataError(const char* msg, int type = 0);
		zoneDataError(std::string msg, int type = 0);
	};
	
	struct zoneInfoError : public containerError {
		zoneInfoError();
		zoneInfoError(const char* msg, int type = 0);
		zoneInfoError(std::string msg, int type = 0);
	};
}
#endif 
