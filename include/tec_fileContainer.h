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



#ifndef TEC_FILE_CONTAINER
#define TEC_FILE_CONTAINER

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "tec_error.h"

namespace tec {
	enum class fileTypeFlag : int32_t {
		full = 0,
		grid = 1,
		solution = 2
	};

	enum class dataTypeFlag : int32_t {
		singlePrecision = 1,
		doublePrecision = 2,
		int32 = 3,
		int16 = 4,
		byte = 5
		//bit = 6
	};

	enum class faceConnMode : int32_t {
		localone2one = 0,
		localone2many = 1,
		globalone2one = 2,
		globalone2many = 3
	};

	enum class formatFlag : char {
		point,
		block
	};

	enum class zoneTypeFlag : uint8_t {
		ordered = 0,
		FEline = 1,
		FEtri = 2,
		FEquad = 3,
		FEtet = 4,
		FEbrick = 5,
		FEpolygon = 6,
		FEpolyhedron = 7
	};

	class zoneInformation {
		friend class fileContainer;
	
		friend class asciiReader;
		friend class szlReader;

		private:
			int nVars;

			int64_t I, J, K, nFaceConns;
			formatFlag dataPacking;
			zoneTypeFlag zoneType;
			faceConnMode faceConnectMode;
			std::string zoneTitle;
			int32_t strandID, shareConnZone;
			double solutionTime;
			
			std::vector<int32_t> zone_varDTs;
			std::vector<int32_t> zone_varLoc;
			std::vector<int32_t> zone_sharedVars;
			std::vector<int32_t> zone_passiveVars;
			
			//using ptrs as face connections may or may not exist and may be 64 or 32 bits
			std::unique_ptr<std::vector<int32_t>> int32_faceConns;
			std::unique_ptr<std::vector<int64_t>> int64_faceConns;

			bool hasFaceConnections;
			bool hasSharedVars;
			bool hasPassiveVars;


			//PRIVATE MEMBER FUNCTIONS (sets zone information
			void set_formatType(char formattingType);
			void set_zoneType(char type);
			void set_faceConnectMode(int32_t mode);
			void set_faceConns(int64_t numConnections, std::vector<int32_t> &faceConnects);
			void set_faceConns(int64_t numConnections, std::vector<int64_t> &faceConnects);
			void set_shareConnZone(int32_t shareZone);
			void set_zoneTitle(std::string title);
			void set_IJKSize(char IJK, int size);
			void set_strandID(int strand);
			void set_solutionTime(double time);

			void set_sharedVar(int vidx, int32_t zidx, int resize = 0);
			void set_passiveVar(int vidx, bool flag, int resize  = 0);
			void set_varDT(int vidx, int32_t type, int resize =  0);
			void set_varLoc(int vidx, int32_t loc, int resize =  0);

		public:
			zoneInformation(int zid, size_t vars);
			zoneInformation(int zid, int vars);
			zoneInformation(zoneInformation &obj);
			zoneInformation(zoneInformation &&obj);
			~zoneInformation();

			const int zoneID;
			
			std::string get_zoneTitle();
			formatFlag get_formattingType();
			zoneTypeFlag get_zoneType();
			faceConnMode get_faceConnectMode();
			int64_t get_numFaceConns();
			int32_t get_shareConnZone();
			int32_t get_strandID();
			double get_solutionTime();
			int get_size(bool node = true);
			int64_t get_Imax();
			int64_t get_Jmax();
			int64_t get_Kmax();

			std::unique_ptr<std::vector<int32_t>> get_varDTs();
			std::unique_ptr<std::vector<int32_t>> get_sharedList();
			std::unique_ptr<std::vector<int32_t>> get_locationList();
			std::unique_ptr<std::vector<int32_t>> get_passiveList();
	};

	class zoneData {
		
		friend class fileContainer;
		
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

		friend class fileContainer;

		friend class asciiReader;
		//friend class asciiWriter;

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
			std::string get_name();
			void resize_zone(int zone, int _size, dataTypeFlag T = dataTypeFlag::singlePrecision);
			zoneData& operator[](int idx); 
	};

	class fileContainer {

		friend class asciiReader;
		friend class asciiWriter;

		friend class szlReader;
		friend class szlWriter;

		private:
			std::string title;
			fileTypeFlag fType;

			std::vector<variable> vars;
			std::vector<zoneInformation> zoneDetails; 
			std::unordered_map<std::string, size_t> var_map;

		public:
			fileContainer();
			~fileContainer();
			
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
