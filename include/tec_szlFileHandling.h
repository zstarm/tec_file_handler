#ifndef TEC_SZL_FILEHANDLE
#define TEC_SZL_FILEHANDLE

#include "tec_error.h"
#include "tec_fileContainer.h"

#include "TECIO.h"

namespace tec {
	class szlReader {
		std::string fname;
		void* szlFileHandle;

		int nZones;
		int nVars;

		void read_currentZone(int zidx, fileContainer &dataContainer, int &err);
		void insert_zoneData(int zidx, int vidx, int nValues, zoneData &subzoneData);

		public:
			szlReader();
			szlReader(std::string _fname);
			szlReader(const char* _fname);
			szlReader(const szlReader &obj) = delete;
			szlReader(szlReader &&obj) = delete;
			~szlReader();

			szlReader& operator=(const szlReader &obj) = delete;
			szlReader& operator=(szlReader &&obj) = delete;

			void read_file(fileContainer &dataContainer);
			void read_file(std::string _fname, fileContainer &dataContainer);
	};

	class szlWriter {
		std::string fname;
		void* szlFileHandle;
		int32_t partition;

		void write_currentZone(int zidx, fileContainer &dataContainer, int &nVars, int &err);
		void write_zoneData(int zidx, int vidx, int nValues, zoneData &subzoneData);

		public:
			szlWriter();
			szlWriter(std::string _fname);
			szlWriter(const char* _fname);
			szlWriter(const szlWriter &obj) = delete;
			szlWriter(szlWriter &&obj) = delete;
			~szlWriter();

			szlWriter& operator=(const szlWriter &obj) = delete;
			szlWriter& operator=(szlWriter &&obj) = delete;
			
			void write_file(fileContainer &dataContainer, bool verbose = true);
			void write_file(std::string _fname, fileContainer &dataContainer, bool verbose = true);

	};
}
#endif
