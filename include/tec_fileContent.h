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
#include <type_traits>
#include <memory>
//#include <filesystem>

//typedef std::variant<float, double, int32_t, int16_t, uint8_t> tecDataTypes;

enum class fileTypeFlag : char {
	full,
	grid,
	solution
};

enum class dataTypeFlag : char {
	//unknown,
	singlePrecision,
	doublePrecision,
	int32,
	int16,
	byte
};

enum class sharedVarFlag : char {
	nonshared,
	shared
};

enum class passiveVarFlag : char {
	nonpassive,
	passive
};

enum class formattingFlag : char {
	point,
	block
};

enum class zoneTypeFlag : char {
	ordered,
	FE
};

class tec_zoneDetails {
	int I, J, K;
	std::vector<dataTypeFlag> DT;
	formattingFlag dataPacking;
	zoneTypeFlag zoneType;
	std::string zoneTitle;
	//std::vector<int> sharedVars;
	int strandID;
	double solutionTime;
	std::vector<int> passiveVars;
	public:
		tec_zoneDetails();
		~tec_zoneDetails();
		
		void set_formatType(char formattingType);
		void set_zoneType(char type);
		void set_zoneTitle(std::string title);
		void set_IJKSize(char IJK, int size);

		formattingFlag get_formattingType();
		zoneTypeFlag get_zoneType();
		int get_size();
		int get_Imax();
		int get_Jmax();
		int get_Kmax();
};

class tec_data {
	protected:
		//int size; 
		dataTypeFlag T;
		std::unique_ptr<std::vector<float>> float_content;
		std::unique_ptr<std::vector<double>> double_content;
		std::unique_ptr<std::vector<int32_t>> int32_content;
		std::unique_ptr<std::vector<int16_t>> int16_content;
		std::unique_ptr<std::vector<uint8_t>> byte_content;

		void allocate(int size);
		template <typename DT> void allocate(int size, DT val);

	public:
		tec_data();
		tec_data(tec_data& obj);
		tec_data(tec_data&& obj);
		tec_data(dataTypeFlag type);
		tec_data(dataTypeFlag type, int size);
		tec_data(int size, float val);
		tec_data(int size, double val);
		tec_data(int size, int32_t val);
		tec_data(int size, int16_t val);
		tec_data(int size, uint8_t val);

		//tec_data(int _size);
		~tec_data();

		//void resize(int new_size, val);
		template <typename DT> void resize(int new_size, DT val);
		void resize(int new_size);
		void push_back(float val);
		void push_back(double val);
		void push_back(int32_t val);
		void push_back(int16_t val);
		void push_back(uint8_t val);

		/*
		void insert(int idx, float val);
		void insert(int idx, double val);
		void insert(int idx, int32_t val);
		void insert(int idx, float val);
		void insert(int idx, float val);
		*/

		//template <typename DT> DT& get(int&& idx);
		//template <typename DT> DT& get(int idx);
		float& fget(int idx);
		double& dget(int idx);
		int32_t& ilget(int idx);
		int16_t& isget(int idx);
		uint8_t& bget(int idx);
		
};

class tec_variable {
	/*
	friend class tec_asciiReader;
	friend class tec_asciiWriter;

	friend class tec_szlReader;
	friend class tec_szlWriter;
	*/

	friend class tec_asciiReader;
	std::string name;
	//std::vector<dataTypeFlag> subzoneDataTypes; 
	
	std::vector<tec_data> subzoneData;

	public:
		tec_variable();
		tec_variable(std::string vname);
		tec_variable(tec_variable & obj);
		tec_variable(tec_variable&& obj);
		~tec_variable();

		//void modify_name(std::string vname);
		void resize_zone(int zone, int _size, dataTypeFlag T = dataTypeFlag::singlePrecision);
		tec_data& operator[](int idx); 
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
