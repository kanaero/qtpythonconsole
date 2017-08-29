#include "qpythonconsole.h"
#include <QColor>
#include <pybind11/embed.h>
#include <iostream>
#include <strstream>
#include <sstream>


namespace py = pybind11;

struct cout_redirect {
	cout_redirect(std::streambuf * new_buffer)
		: old(std::cout.rdbuf(new_buffer))
	{ }

	~cout_redirect() {
		std::cout.rdbuf(old);
	}

private:
	std::streambuf * old;
};

struct cerr_redirect {
	cerr_redirect(std::streambuf * new_buffer)
		: old(std::cerr.rdbuf(new_buffer))
	{ }

	~cerr_redirect() {
		std::cerr.rdbuf(old);
	}

private:
	std::streambuf * old;
};

QPythonConsole::QPythonConsole(QWidget *parent, const QString &welcomeText) : QConsole(parent, welcomeText) {
	connect(
		this, &QConsole::execCommand,
		this, &QPythonConsole::ExecuteAndPrintResults
	);
	this->setPrompt(">>>");
	QFont f("unexistent");
	f.setStyleHint(QFont::Monospace);
	this->setFont(f);



}

QPythonConsole::~QPythonConsole() {

}


void QPythonConsole::ExecuteAndPrintResults(const QString &command) {
	QByteArray ba = command.toLocal8Bit();
	std::stringbuf  coutstream;
	std::stringbuf  cerrstream;

	cout_redirect cout_guard(&coutstream);
	cerr_redirect cerr_guard(&cerrstream);
	try {
		auto result = py::eval<py::eval_single_statement>(ba.data());
		this->printCommandExecutionResults(coutstream.str().c_str(), QConsole::Complete);
	}
	catch (py::error_already_set &e) {
		this->printCommandExecutionResults(e.what(), QConsole::Error);
	}
}
