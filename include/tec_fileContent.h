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

#include <iostream>
#include <string>
#include <vector>
//#include <type_traits>
#include <unordered_map>
#include <memory>

#include "tec_error.h"

enum class fileTypeFlag : char {
	full = '0',
	grid = '1',
	solution = '2'
};

enum class dataTypeFlag : int32_t {
	singlePrecision = 1,
	doublePrecision = 2,
	int32 = 3,
	int16 = 4,
	byte = 5,
	bit = 6
};

enum class sharedVarFlag : bool {
	nonshared = false,
	shared = true
};

enum class passiveVarFlag : bool {
	nonpassive = false,
	passive = true
};

enum class formattingFlag : char {
	point = '0',
	block = '1'
};

enum class zoneTypeFlag : char {
	ordered = '0',
	FE = '1'
};

class tec_zoneDetails {
	friend class tec_fileContent;
	
	int nVars;

	int I, J, K;
	formattingFlag dataPacking;
	zoneTypeFlag zoneType;
	std::string zoneTitle;

	int strandID;
	double solutionTime;

	bool hasSharedVars;
	bool hasPassiveVars;

	std::vector<dataTypeFlag> zone_varDTs;
	std::vector<int32_t> zone_sharedVars;
	std::vector<passiveVarFlag> zone_passiveVars;
	
	public:
		tec_zoneDetails(int zid, size_t vars);
		tec_zoneDetails(tec_zoneDetails &obj);
		tec_zoneDetails(tec_zoneDetails &&obj);
		~tec_zoneDetails();

		const int zoneID;
		
		void set_formatType(char formattingType);
		void set_zoneType(char type);
		void set_zoneTitle(std::string title);
		void set_IJKSize(char IJK, int size);
		void set_strandID(int strand);
		void set_solutionTime(double time);

		void set_sharedVar(int vidx, int32_t zidx, bool push = false);
		void set_passiveVar(int vidx, passiveVarFlag flag, bool push = false);
		void set_varDT(int vidx, dataTypeFlag type, bool push =  false);

		formattingFlag get_formattingType();
		zoneTypeFlag get_zoneType();
		int get_size();
		int get_Imax();
		int get_Jmax();
		int get_Kmax();

		std::vector<dataTypeFlag>* get_varDTs();
		std::vector<int32_t>* get_sharedList();
		//std::vector<passiveVarFlag>* get_passiveList();
};

class tec_data {
	
	friend class tec_fileContent;
	
	private:
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

		~tec_data();

		template <typename DT> void resize(int new_size, DT val);
		void resize(int new_size);
		void push_back(float val);
		void push_back(double val);
		void push_back(int32_t val);
		void push_back(int16_t val);
		void push_back(uint8_t val);

		dataTypeFlag type();
		int get_array_size();

		float& get_float(int idx);
		double& get_double(int idx);
		int32_t& get_int32(int idx);
		int16_t& get_int16(int idx);
		uint8_t& get_byte(int idx);
		
};

class tec_variable {

	friend class tec_fileContent;

	friend class tec_asciiReader;
	friend class tec_asciiWriter;

	//friend class tec_szlReader;
	//friend class tec_szlWriter;


	std::string name;
	
	std::vector<tec_data> subzoneData;

	public:
		tec_variable();
		tec_variable(std::string vname);
		tec_variable(tec_variable &obj);
		tec_variable(tec_variable &&obj);
		~tec_variable();

		void modify_name(std::string vname);
		void resize_zone(int zone, int _size, dataTypeFlag T = dataTypeFlag::singlePrecision);
		tec_data& operator[](int idx); 
};

class tec_fileContent {

	friend class tec_asciiReader;
	friend class tec_asciiWriter;

	//friend class tec_szlReader;
	//friend class tec_szlWriter;

	private:
		std::string title;
		fileTypeFlag fType;

		std::vector<tec_variable> variables;
		std::vector<tec_zoneDetails> zoneDetails; 
		std::unordered_map<std::string, size_t> var_map;

	public:
		tec_fileContent();
		~tec_fileContent();
		
		void print_headerDetails();
		void print_zoneDetails(int zidx);
		void print_zoneData(int zidx);
		void print_fileDetails(bool include_data=false);
		
};

#endif 
