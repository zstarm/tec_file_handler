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

namespace tec {
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
	/*
	enum class sharedVarFlag : bool {
		nonshared = false,
		shared = true
	};

	enum class passiveVarFlag : bool {
		nonpassive = false,
		passive = true
	};
	*/

	enum class formattingFlag : char {
		point,
		block
	};

	enum class zoneTypeFlag : char {
		ordered,
		FE
	};

	class zoneDetails {
		friend class fileContent;
		friend class asciiReader;

		private:
			int nVars;
			int I, J, K;
			formattingFlag dataPacking;
			zoneTypeFlag zoneType;
			std::string zoneTitle;
			int strandID;
			double solutionTime;
			std::vector<int32_t> zone_varDTs;
			std::vector<int32_t> zone_sharedVars;
			std::vector<bool> zone_passiveVars;

			bool hasSharedVars;
			bool hasPassiveVars;

			void set_formatType(char formattingType);
			void set_zoneType(char type);
			void set_zoneTitle(std::string title);
			void set_IJKSize(char IJK, int size);
			void set_strandID(int strand);
			void set_solutionTime(double time);

			void set_sharedVar(int vidx, int32_t zidx, int resize = 0);
			void set_passiveVar(int vidx, bool flag, int resize  = 0);
			void set_varDT(int vidx, int32_t type, int resize =  0);

		public:
			zoneDetails(int zid, size_t vars);
			zoneDetails(zoneDetails &obj);
			zoneDetails(zoneDetails &&obj);
			~zoneDetails();

			const int zoneID;
			
			formattingFlag get_formattingType();
			zoneTypeFlag get_zoneType();
			int get_size();
			int get_Imax();
			int get_Jmax();
			int get_Kmax();

			std::unique_ptr<std::vector<int32_t>> get_varDTs();
			std::unique_ptr<std::vector<int32_t>> get_sharedList();
			std::unique_ptr<std::vector<bool>> get_passiveList();
	};

	class zoneData {
		
		friend class fileContent;
		
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
			zoneData();
			zoneData(zoneData& obj);
			zoneData(zoneData&& obj);
			zoneData(dataTypeFlag type);
			zoneData(dataTypeFlag type, int size);
			zoneData(int size, float val);
			zoneData(int size, double val);
			zoneData(int size, int32_t val);
			zoneData(int size, int16_t val);
			zoneData(int size, uint8_t val);

			~zoneData();

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

	class variable {

		friend class fileContent;

		friend class asciiReader;
		friend class asciiWriter;

		//friend class szlReader;
		//friend class szlWriter;


		std::string name;
		
		std::vector<zoneData> subzoneData;

		public:
			variable();
			variable(std::string vname);
			variable(variable &obj);
			variable(variable &&obj);
			~variable();

			void modify_name(std::string vname);
			void resize_zone(int zone, int _size, dataTypeFlag T = dataTypeFlag::singlePrecision);
			zoneData& operator[](int idx); 
	};

	class fileContent {

		friend class asciiReader;
		friend class asciiWriter;

		//friend class szlReader;
		//friend class szlWriter;

		private:
			std::string title;
			fileTypeFlag fType;

			std::vector<variable> variables;
			std::vector<zoneDetails> zoneDetails; 
			std::unordered_map<std::string, size_t> var_map;

		public:
			fileContent();
			~fileContent();
			
			void print_headerDetails();
			void print_zoneDetails(int zidx);
			void print_zoneData(int zidx);
			void print_fileDetails(bool include_data=false);

			int get_numZones();
			int get_numVariables();


			void operator[](int vidx);
			void operator[](std::string vname);
	};
}


#endif 
