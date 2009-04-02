#include "defaultsimulation.h"
#include <model.h>
#include <node.h>
#include <flow.h>
#include <boost/foreach.hpp>
#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <map>

CD3_DECLARE_SIMULATION_NAME(DefaultSimulation)

struct SimPriv {
	int runDT;
};

DefaultSimulation::DefaultSimulation() {
	sp = new SimPriv();
}

DefaultSimulation::~DefaultSimulation() {
	delete sp;
}

int DefaultSimulation::run(int time, int dt) {
	node_set_type sources = model->getSourceNodes();

	std::map<Node *, int> deps = createDependsMap();
	BOOST_FOREACH(Node *n, sources) {
		run(n, time, deps);
	}
	return dt;
}

//typedef std::vector<next_node_type> vector_next_node_type;

void DefaultSimulation::run(Node *n, int time, std::map<Node *, int> &depends) {
	n->f(time, sim_param.dt);
	BOOST_FOREACH(next_node_type con, model->forward(n)) {
		std::string src_port, snk_port;
		Node *next;
		boost::tuples::tie(src_port, next, snk_port) = con;

		next->setInPort(snk_port, n->getOutPort(src_port));

		depends[next]--;

		if (depends[next] > 0) {
			return;
		}
		run(next, time, depends);
	}
	return;
}

std::map<Node *, int> DefaultSimulation::createDependsMap() const {
	std::map<Node *, int> deps;

	BOOST_FOREACH(Node *node, *model->getNodes()) {
		deps[node] = model->backward(node).size();
	}

	return deps;
}
