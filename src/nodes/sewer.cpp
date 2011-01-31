#include "sewer.h"

#include <flow.h>
#include <flowfuns.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

using namespace boost;

CD3_DECLARE_NODE_NAME(Sewer)

Sewer::Sewer() {
	addInPort(ADD_PARAMETERS(in));
	addOutPort(ADD_PARAMETERS(out));
	addParameter(ADD_PARAMETERS(K))
		.setUnit("s");
	addParameter(ADD_PARAMETERS(X));
	addParameter(ADD_PARAMETERS(N));
	K = 300;
	X = 0.1;
	N = 11;
}

Sewer::~Sewer() {
}

void Sewer::deinit() {
	for (int i = 0; i < N; i++) {
		Flow *f = &getState<Flow>(str(format("V[%1%]") % i));
		removeState(str(format("V[%1%]") % i));
		delete f;
	}
	V.clear();
}

bool Sewer::init(ptime start, ptime end, int dt) {
	(void) end;
	for (int i = 0; i < N; i++) {
		V.push_back(new Flow());
		addState(str(format("V[%1%]") % i), V[i]);
	}

	addMuskParam(dt);
	return true;
}


int Sewer::f(ptime time, int dt) {
	(void) time;
	double C_x, C_y;
	setMuskParam(&C_x, &C_y, dt);

	Flow tmp = in;

	for (int i = 0; i < N; i++) {
		tmp = FlowFuns::route_sewer(tmp, V[i], C_x, C_y, dt);
	}
	out = tmp;

	return dt;
}

void Sewer::addMuskParam(int dt) {
	double dt_halve = static_cast<double>(dt) / 2;
	double K_1_min_X = K*(1-X);
	double Cx = (dt_halve - K*X) / (dt_halve + K_1_min_X);
	double Cy = 1.0 / (dt_halve+K_1_min_X);
	musk_param[dt] = std::pair<double, double>(Cx, Cy);
}

void Sewer::setMuskParam(double *x, double *y, int dt) {
	if (musk_param.find(dt) == musk_param.end()) {
		addMuskParam(dt);
	}
	*x = musk_param[dt].first;
	*y = musk_param[dt].second;
}
