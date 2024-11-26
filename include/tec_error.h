#ifndef TEC_ERROR
#define TEC_ERROR

struct tec_asciiReaderError {
	const char* error_msg;
	tec_asciiReaderError();
	tec_asciiReaderError(const char* msg);
	const char* what() const noexcept;
};

struct tec_containerError  {
	const char* error_msg;
	tec_containerError();
	tec_containerError(const char* msg);
	const char* what() const noexcept;
};


#endif 
