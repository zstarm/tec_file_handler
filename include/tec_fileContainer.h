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

#include "tec_error.h"

#include "tec_enumerations.h"
#include "tec_zoneInformation.h"
#include "tec_zoneData.h"
#include "tec_variable.h"

namespace tec {

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
			fileContainer(const fileContainer &obj) = default;
			fileContainer(fileContainer &&obj) = default;
			~fileContainer();

			fileContainer& operator=(const fileContainer &obj) = default;
			fileContainer& operator=(fileContainer &&obj) = default;
			
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
