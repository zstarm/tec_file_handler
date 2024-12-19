#ifndef TEC_ZONEDATA
#define TEC_ZONEDATA

#include <iostream>
#include <vector>
#include <memory>

#include "tec_enumerations.h"
#include "tec_error.h"

namespace tec {
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
			zoneData(const zoneData& obj);
			zoneData(zoneData&& obj);
		
			zoneData(dataTypeFlag type);
			zoneData(dataTypeFlag type, int size);
		
			zoneData(int size, float val);
			zoneData(int size, double val);
			zoneData(int size, int32_t val);
			zoneData(int size, int16_t val); 
			zoneData(int size, uint8_t val);
			
			zoneData(std::vector<float> &&data);
			zoneData(std::vector<double> &&data);
			zoneData(std::vector<int32_t> &&data);
			zoneData(std::vector<int16_t> &&data);
			zoneData(std::vector<uint8_t> &&data);
		
			~zoneData();

			zoneData& operator=(const zoneData& obj); //copy assignment
			zoneData& operator=(zoneData &&obj); //move assignment

			template <typename DT> 
			void resize(int new_size, DT val) {
				switch(T) {
					case dataTypeFlag::singlePrecision:
						if(float_content) {
							std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
							float_content.reset();
						}
						break;
					case dataTypeFlag::doublePrecision:
						if(double_content) {
							std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
							double_content.reset();
						}
						break;
					case dataTypeFlag::int32:
						if(int32_content) {
							std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
							int32_content.reset();
						}
						break;
					case dataTypeFlag::int16:
						if(int16_content) {
							std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
							int16_content.reset();
						}
						break;
					case dataTypeFlag::byte:
						if(byte_content) {
							std::cout << "WARNING!: potentially deleting previous data/container" << std::endl;
							byte_content.reset();
						}
						break;
					default:
						std::cout << "ERROR!: unrecognized data type for zoneData, defaulting to \"float\" data type" << std::endl;
				}

				uint8_t tmpb;
				int16_t tmpis;
				int32_t tmpil;
				double tmpd;
				if(std::is_same<DT, uint8_t>::value) {
					T = dataTypeFlag::byte;
				}
				else if(std::is_same<DT, int16_t>::value) {
					T = dataTypeFlag::int16;
				}
				else if(std::is_same<DT,int32_t>::value) {
					T = dataTypeFlag::int32;
				}
				else if(std::is_same<DT,double>::value) {
					T = dataTypeFlag::doublePrecision;
				}
				else {
					T = dataTypeFlag::singlePrecision;
				}
				allocate<DT>(new_size, val); 
			}
		
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
}

#endif
