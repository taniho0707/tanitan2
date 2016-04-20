#pragma once

#include <array>
#include <map>

namespace slalomparams{
	using namespace std;

	enum class RunType : uint8_t {
		TRAPACCEL,
		PIVOTTURN,
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
	};

	struct pack{
		float d_before;
		float d_after;
		float acc_rad;
		float max_deg;
		float in_vel;
		float out_vel;
		float min_vel;
		float acc_lin;
	};

	typedef struct pack pack_t;

	const static map<uint16_t, pack_t> SLALOM90SML_RIGHT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		{0.3, {0.005, 0.005,    70,     30,    0.3,     0.3,     0.3,     0.0}},
	};
	
	const static map<uint16_t, pack_t> SLALOM90SML_LEFT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		{0.3, {0.005, 0.005,    70,     30,    0.3,     0.3,     0.3,     0.0}},
	};
	
	const static map<uint16_t, pack_t> SLALOM90_RIGHT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};

	const static map<uint16_t, pack_t> SLALOM90_LEFT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM180_RIGHT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM180_LEFT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM45IN_RIGHT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM45IN_LEFT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM45OUT_RIGHT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM45OUT_LEFT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM135IN_RIGHT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM135IN_LEFT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM135OUT_RIGHT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM135OUT_LEFT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM90OBL_RIGHT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static map<uint16_t, pack_t> SLALOM90OBL_LEFT = {
	//	{vel, {d_bef, d_aft, a_rad, maxdeg, in_vel, out_vel, min_vel, acc_lin}},
		
	};
	
	const static array< const map<uint16_t,pack_t>* , 16> params = {
		&SLALOM90SML_RIGHT,
		&SLALOM90SML_LEFT,
		&SLALOM90_RIGHT,
		&SLALOM90_LEFT,
		&SLALOM180_RIGHT,
		&SLALOM180_LEFT,
		&SLALOM45IN_RIGHT,
		&SLALOM45IN_LEFT,
		&SLALOM45OUT_RIGHT,
		&SLALOM45OUT_LEFT,
		&SLALOM135IN_RIGHT,
		&SLALOM135IN_LEFT,
		&SLALOM135OUT_RIGHT,
		&SLALOM135OUT_LEFT,
		&SLALOM90OBL_RIGHT,
		&SLALOM90OBL_LEFT,
	};
}
