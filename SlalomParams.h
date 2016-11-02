#pragma once

#include <array>
#include <map>

namespace slalomparams{
	enum class RunType : uint8_t {
		SLALOM90SML_RIGHT,
		SLALOM90SML_LEFT,
		SLALOM90_RIGHT,
		SLALOM90_LEFT,
		SLALOM180_RIGHT,
		SLALOM180_LEFT,
		SLALOM45IN_RIGHT,
		SLALOM45IN_LEFT,
		SLALOM45OUT_RIGHT,
		SLALOM45OUT_LEFT,
		SLALOM135IN_RIGHT,
		SLALOM135IN_LEFT,
		SLALOM135OUT_RIGHT,
		SLALOM135OUT_LEFT,
		SLALOM90OBL_RIGHT,
		SLALOM90OBL_LEFT,
		TRAPACCEL,
		PIVOTTURN,
		TRAPDIAGO,
	};

	struct pack{
		float d_before;
		float d_after;
		float acc_rad;
		float deg;
		float const_deg;
		float in_vel;
		float out_vel;
		float min_vel;
		float acc_lin;
	};

	typedef struct pack pack_t;

	using namespace std;

	array< map<float,pack_t>* , 16> getParams();
}
