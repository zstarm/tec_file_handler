#ifndef TEC_ENUMERATE
#define TEC_ENUMERATE

#include "stdint.h"

namespace tec {
	enum class fileTypeFlag : int32_t {
		full = 0,
		grid = 1,
		solution = 2
	};

	enum class dataTypeFlag : int32_t {
		empty = 0,
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
}
#endif
