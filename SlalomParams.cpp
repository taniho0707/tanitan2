#include "SlalomParams.h"

using namespace std;
using namespace slalomparams;

array< map<float,pack_t>* , 16> slalomparams::getParams(){
	static map<float, pack_t> SLALOM90SML_RIGHT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{0.25, {0.010, 0.01,  9500,95.0,     30,   0.25,    0.25,    0.25,     0.0}},
		{ 0.3, {0.022,0.010, 14030,93.5,     30,    0.3,     0.3,     0.3,     0.0}},
	};
	
	static map<float, pack_t> SLALOM90SML_LEFT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{0.25, {0.010, 0.01,  9500,92.2,     30,   0.25,    0.25,    0.25,     0.0}},
		{ 0.3, {0.020,0.009, 14030,93.3,     30,    0.3,     0.3,     0.3,     0.0}},
	};
	
	static map<float, pack_t> SLALOM90_RIGHT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.3, {0.025,0.032,   3000,93.4,     30,    0.3,     0.3,     0.3,     0.0}},
		{ 0.5, {0.015,0.000,   8000,93.4,     30,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.000,0.022,  13000,93.4,     30,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM90_LEFT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.3, {0.032,0.022,   3000,93.4,     30,    0.3,     0.3,     0.3,     0.0}},
		{ 0.5, {0.021,0.000,   8000,93.4,     30,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.000,0.022,  13000,93.4,     30,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM180_RIGHT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.3, {0.015,0.022,   1600, 182,     60,    0.3,     0.3,     0.3,     0.0}},
		{ 0.5, {0.000,0.000,   4400, 181,     60,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.000,0.022,   8400, 182,     60,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM180_LEFT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.3, {0.015,0.022,   1600, 180,     60,    0.3,     0.3,     0.3,     0.0}},
		{ 0.5, {0.000,0.000,   4400, 182,     60,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.000,0.022,   8400, 182,     60,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM45IN_RIGHT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.002,0.000,   8000,47.0,     15,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.002,0.000,  20000,47.5,     23,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM45IN_LEFT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.010,0.000,   8000,46.5,     15,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.002,0.000,  17000,47.0,     20,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM45OUT_RIGHT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.020,0.000,   8000,46.5,     15,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.015,0.000,  17000,47.0,     20,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM45OUT_LEFT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.020,0.000,   8000,46.5,     15,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.010,0.000,  17000,47.0,     20,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM135IN_RIGHT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.025,0.000,  11500, 138,     45,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.002,0.000,  15500, 137,     65,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM135IN_LEFT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.025,0.000,  11500, 137,     45,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.002,0.000,  15500, 137,     65,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM135OUT_RIGHT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.030,0.030,  11500, 140,     45,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.001,0.000,  18000, 136,     67,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM135OUT_LEFT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.025,0.030,  11500, 137,     45,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.001,0.000,  18000, 136,     67,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM90OBL_RIGHT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.015, 0.000, 16000,94.4,     30,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.002, 0.010, 26000,93.8,     45,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static map<float, pack_t> SLALOM90OBL_LEFT = {
	//  { vel, {d_bef, d_aft, a_rad, deg, cnsdeg, in_vel, out_vel, min_vel, acc_lin}},
		{ 0.5, {0.015, 0.000, 16000,94.4,     30,    0.5,     0.5,     0.5,     0.0}},
		{ 0.7, {0.002, 0.010, 26000,94.4,     45,    0.7,     0.7,     0.7,     0.0}},
	};
	
	static array< map<float,pack_t>* , 16> params = {
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

	return params;
}
