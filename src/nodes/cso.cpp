#include "cso.h"

#include <boost/foreach.hpp>
#include <boost/format.hpp>

using namespace boost;

#include <cd3assert.h>

CD3_DECLARE_NODE_NAME(CSO)

CSO::CSO() {
	addInPort(ADD_PARAMETERS(in));
	addOutPort(ADD_PARAMETERS(out));
	addOutPort(ADD_PARAMETERS(overflow));

	V_Max = 300;
	Q_Max = 0.1;

	addParameter(ADD_PARAMETERS(V_Max))
		.setUnit("m3");
	addParameter(ADD_PARAMETERS(Q_Max))
		.setUnit("m3/s");

	addState(ADD_PARAMETERS(stored_volume));
}

CSO::~CSO() {
}

bool CSO::init(ptime start, ptime end, int dt) {
	(void) end;
	(void) dt;
	this->start = start;
	return true;
}

int CSO::f(ptime time, int dt) {
	if (time == start) {
		stored_volume.clear();
	}

	double Q_In = in[0];
	double V_Stored = stored_volume[0];
	double V_Volume = (Q_In - Q_Max) * dt + V_Stored;
	double Q_Out = 0;
	double Q_Overflow = 0;
	double V_Stored_new = 0;

	if (V_Volume > 0) {
		if (V_Volume < V_Max) {
			//Case 3
			V_Stored_new = (Q_In - Q_Max) * dt - V_Stored;
			Q_Out = Q_Max;
			Q_Overflow = 0;
		} else {
			//Case 2
			V_Stored_new = V_Max;
			Q_Out = Q_Max;
			Q_Overflow = Q_In - Q_Max - (V_Max - V_Stored) / dt;
		}
	} else { //V_Volume < 0
		//Case 1
		V_Stored_new = 0;
		Q_Out = V_Stored  / dt + Q_In;
		Q_Overflow = 0;
	}

	out[0] = Q_Out;
	overflow[0] = Q_Overflow;
	stored_volume[0] = V_Stored_new;

	double V_Prime = Q_In * dt + V_Stored;

	if (V_Prime > 0) {

		cd3assert(V_Prime > 0, str(format("V_Prime (%1% needs to be bigger than 0") % V_Prime));
		double V_Prime_Inv = 1 / V_Prime;

		for (size_t i = 0; i < out.countUnits(Flow::concentration); i++) {
			double Ci = (in[i+1] * Q_In * dt
						+ stored_volume[i+1] * V_Stored) * V_Prime_Inv;
			out[i+1] = Ci;
			overflow[i+1] = Ci;
			stored_volume[i+1] = Ci;
		}
	} else {
		Logger(Warning) << "V_prime < 0 ";
	}

	return dt;
}
