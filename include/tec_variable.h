#ifndef TEC_VARIABLE
#define TEC_VARIABLE

#include <string>
#include <vector>

#include "tec_enumerations.h"
#include "tec_error.h"
#include "tec_zoneData.h"

namespace tec {
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
			variable(variable &obj) = delete;
			variable(variable &&obj);
			~variable();

			void modify_name(std::string vname);
			std::string get_name();
			void resize_zone(int zone, int _size, dataTypeFlag T = dataTypeFlag::singlePrecision);
			zoneData& operator[](int idx); 
	};

}
#endif
