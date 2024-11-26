#ifndef TEC_EXCEPT
#define TEC_EXCEPT

#include <exception>

struct tec_readerException : public std::exception {
	const char* error_msg;
	tec_readerException();
	tec_readerException(const char* msg);
	const char* what() const noexcept;
};

struct tec_containerException : public std::exception {
	const char* error_msg;
	tec_readerException();
	tec_readerException(const char* msg);
	const char* what() const noexcept;
};


#endif 
