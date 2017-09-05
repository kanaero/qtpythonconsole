#include "qpythonconsole.h"
#include <QColor>
#include <pybind11/embed.h>
#include <iostream>
#include <strstream>
#include <sstream>
#include <pybind11/iostream.h>

namespace py = pybind11;



static QString out_text;
class Redirector {
public:
	Redirector() {}
	virtual ~Redirector() {}
	void write(std::string text) {
		out_text.append(text.c_str());
	}

	void Clear() {
		out_text.clear();
	}
	static Redirector& Get() {
		static Redirector global_instance;
		return global_instance;
	}
};

PYBIND11_EMBEDDED_MODULE(redirector, m) {
	m.doc() = "pybind11 stdout/stderr redirection module";
	py::class_<Redirector>(m, "Redirector")
		.def("write", &Redirector::write)
		.def_static("Get", &Redirector::Get);
}



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
	
	try {
		py::exec("import redirector");
		py::exec("import sys");
		py::exec("sys.stdout = redirector.Redirector.Get()");
		py::exec("sys.stderr = redirector.Redirector.Get()");
	} catch (py::error_already_set &e) {
		this->printCommandExecutionResults(e.what(), QConsole::Error);
	}
}

QPythonConsole::~QPythonConsole() {

}


void QPythonConsole::ExecuteAndPrintResults(const QString &command) {
	QByteArray ba = command.toLocal8Bit();
	std::stringbuf  coutstream;
	std::stringbuf  cerrstream;
	std::stringbuf  stdoutstream;

	cout_redirect cout_guard(&coutstream);
	cerr_redirect cerr_guard(&cerrstream);
	try {
		if (command.isEmpty()) {
			auto result = py::eval<py::eval_statements>(ba.data());
		}
		else {
			auto result = py::eval<py::eval_single_statement>(ba.data());
		}
		
		this->printCommandExecutionResults(out_text, QConsole::Complete);
		Redirector::Get().Clear();
	}
	catch (py::error_already_set &e) {
		this->printCommandExecutionResults(e.what(), QConsole::Error);
		Redirector::Get().Clear();
	}
}
