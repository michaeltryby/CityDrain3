#include "newsimulationdialog.h"
#include <ui_newsimulationdialog.h>
#include <simulationregistry.h>
#include <noderegistry.h>
#include <simulation.h>
#include <simulationscene.h>
#include <flow.h>

#include <QFile>
#include <QString>
#include <boost/foreach.hpp>

NewSimulationDialog::NewSimulationDialog(SimulationRegistry *registry,
										 QWidget *parent,
										 Qt::WindowFlags f)
	: QDialog(parent, f), registry(registry), ui(new Ui::NewSimulationDialog()) {
	ui->setupUi(this);
	QStringList list;

	registry->addNativePlugin("nodes");

	BOOST_FOREACH(std::string name, registry->getRegisteredNames()) {
		list << QString::fromStdString(name);
	}
	ui->simulationComboBox->addItems(list);
}



void NewSimulationDialog::defineFlow() {
	using namespace std;
	map<string, Flow::CalculationUnit> definition;
	definition[ui->flowName->text().toStdString()] = Flow::flow;
	Q_FOREACH(QString c, ui->concentrationNames->text().split(' ', QString::SkipEmptyParts)) {
		definition[c.trimmed().toStdString()] = Flow::concentration;
	}
	Flow::undefine();
	Flow::define(definition);
}

ISimulation *NewSimulationDialog::createSimulation() {
	SimulationParameters p(qttopt(ui->start->dateTime()),
						   qttopt(ui->stop->dateTime()),
						   ui->dt->value());
	ISimulation *sim = registry->createSimulation(ui->simulationComboBox->currentText().toStdString());
	sim->setSimulationParameters(p);

	defineFlow();
	return sim;
}

void NewSimulationDialog::on_start_dateTimeChanged(const QDateTime &date) {
	ui->stop->setMinimumDateTime(date.addSecs(ui->dt->value()));
}
