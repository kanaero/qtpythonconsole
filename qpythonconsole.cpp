#include "qpythonconsole.h"
#include <QColor>
#include <pybind11/embed.h>
namespace py = pybind11;

QPythonConsole::QPythonConsole(QWidget *parent, const QString &welcomeText) : QConsole(parent, welcomeText) {
	connect(
		this, &QConsole::execCommand,
		this, &QPythonConsole::ExecuteAndPrintResults
	);
	this->setPrompt(">>");
	QFont f("unexistent");
	f.setStyleHint(QFont::Monospace);
	this->setFont(f);



}

QPythonConsole::~QPythonConsole() {

}


void QPythonConsole::ExecuteAndPrintResults(const QString &command) {
	QByteArray ba = command.toLocal8Bit();
	try {
		auto result = py::eval(ba.data());
		this->printCommandExecutionResults(command, QConsole::Complete);

	}
	catch (py::error_already_set &e) {
		this->printCommandExecutionResults(command, QConsole::Complete);

	}
}
