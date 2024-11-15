//-------------------------------------------------------------------------------------------------
// TECPLOT FILE DATA CONTAINERS
//
// Author: Zach Starman
// Latest Date: November 12, 2024
//
// Description: classes to contain and interact with data from tecplot files (SZL and DAT) 
//		* provides functionality to read or write data from/to .szplt and .dat tecplot file types
//		* stores data variables in std::vectors based on subzones
//-------------------------------------------------------------------------------------------------



#ifndef TEC_FILE_CONTENT
#define TEC_FILE_CONTENT

//#include "TECIO.h"

#include <iostream>
#include <string>
#include <vector>
//#include <filesystem>


enum class fileTypeFlag : uint8_t {
	full,
	grid,
	solution
};

enum class dataTypeFlag : uint8_t {
	unknown,
	singlePrecision,
	doublePrescision,
	int32,
	int16,
	byte
};

enum class sharedVarFlag : uint8_t {
	shared,
	nonshared
};

enum class passiveVarFlag : uint8_t {
	passive,
	nonpassive
};

class tec_zoneDetails {
	int I, J, K;
	std::vector<dataTypeFlag> DT;
	std::string datapacking;
	//std::vector<int> sharedVars;
	int strandID;
	double solutionTime;
	std::vector<int> passiveVars;
	public:
		tec_zoneDetails();
		~tec_zoneDetails();
};

class tec_data {
	protected:
		int size; 
	public:
		tec_data();
		~tec_data();

		//virtual void resize(int new_size);
		//virtual double& operator[](int & idx); 
		
};

/*
class tec_floatType : public tec_data {
	private:
		std::vector<float> val;

	public:
		tec_floatType();
		~tec_floatType();

		//float& operator[](int& idx);
};

class tec_doubleType : public tec_data {
	private:
		std::vector<double> val;
	public:
		tec_doubleType();
		tec_doubleType(int _size);
		~tec_doubleType();

		void resize(int new_size) override;
		double& operator[](int & idx) override; 
};

class tec_32intType : public tec_data {
	public:
		std::vector<int32_t> val;
};

class tec_16intType : public tec_data {
	public:
		std::vector<int16_t> val;
};

class tec_byteType : public tec_data {
	public:
		std::vector<uint8_t> val;
};
*/

class tec_variable {
	/*
	friend class tec_asciiReader;
	friend class tec_asciiWriter;

	friend class tec_szlReader;
	friend class tec_szlWriter;
	*/

	friend class tec_asciiReader;
	std::string name;
	std::vector<tec_data*> zone_data;
	public:
		tec_variable();
		tec_variable(std::string vname);
		~tec_variable();

		//void modify_name(std::string vname);
};

class tec_fileContent {

	friend class tec_asciiReader;
	friend class tec_asciiWriter;

	friend class tec_szlReader;
	friend class tec_szlWriter;

	private:
		std::string title;
		fileTypeFlag fType;

		std::vector<tec_variable> variables;
		std::vector<tec_zoneDetails> zoneDetails; 

	public:
		tec_fileContent();
		//tec_fileContent(std::string _fname, uint8_t flags = 0);
		
		~tec_fileContent();
		
};

#endif 
