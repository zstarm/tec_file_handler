#ifndef TEC_ZONEINFO
#define TEC_ZONEINFO

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "tec_error.h"
#include "tec_enumerations.h"

namespace tec {
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
}
#endif
