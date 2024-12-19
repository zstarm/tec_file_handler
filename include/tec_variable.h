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
		
		friend class szlReader;
		//friend class szlWriter;

		std::string name;
		std::vector<zoneData> subzoneData;
		
		void add_emptyInstances(int numInstances);
		void resize_zone(int zone, int _size, dataTypeFlag T = dataTypeFlag::singlePrecision);
		
		public:
			variable();
			variable(std::string vname);
			variable(std::string vname, std::vector<float> &&fdata, int zidx = 0);
			variable(std::string vname, std::vector<double> &&ddata, int zidx = 0);
			variable(std::string vname, std::vector<int32_t> &&ldata, int zidx = 0);
			variable(std::string vname, std::vector<int16_t> &&sdata, int zidx = 0);
			variable(std::string vname, std::vector<uint8_t> &&bdata, int zidx = 0);
			
			variable(const variable &obj) = default;
			variable(variable &&obj) = default;
			~variable();

			variable& operator=(const variable &obj) = default;
			variable& operator=(variable &&obj) = default;

			void modify_name(std::string vname);
			std::string& get_name();

			void add_zoneData(int zidx, std::vector<float> &&data);
			void add_zoneData(int zidx, std::vector<double> &&data);
			void add_zoneData(int zidx, std::vector<int32_t> &&data);
			void add_zoneData(int zidx, std::vector<int16_t> &&data);
			void add_zoneData(int zidx, std::vector<uint8_t> &&data);
			
			zoneData& operator[](int idx); 
	};

}
#endif
