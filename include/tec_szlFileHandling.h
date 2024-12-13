#ifndef TEC_SZL_FILEHANDLE
#define TEC_SZL_FILEHANDLE

#include "tec_error.h"
#include "tec_fileContent.h"

#include "TECIO.h"

namespace tec {
	class szlReader {
		std::string fname;
		void* szlFileHandle;

		int nZones;
		int nVars;

		void read_currentZone(int zidx, fileContent &dataContainer, int &err);
		void insert_zoneData(int zidx, int vidx, int nValues, zoneData &subzoneData);

		public:
			szlReader();
			szlReader(std::string _fname);
			szlReader(const char* _fname);
			~szlReader();

			void read_file(fileContent &dataContainer);
			void read_file(std::string _fname, fileContent &dataContainer);
	};

	class szlWriter {
		std::string fname;
		void* szlFileHandle;

		public:
			szlWriter();
			szlWriter(std::string _fname);
			szlWriter(const char* _fname);
			~szlWriter();

			void write_file(fileContent &dataContainer);
			void write_file(std::string _fname, fileContent &dataContainer);

	};
}
#endif
