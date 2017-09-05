#include "qpythonconsole.h"
#include <QColor>
#include <pybind11/embed.h>
#include <iostream>
#include <strstream>
#include <sstream>
#include <pybind11/iostream.h>

namespace py = pybind11;
const std::string blah = R"(
from contextlib import contextmanager
import ctypes
import io
import os, sys
import tempfile

libc = ctypes.CDLL(None)
c_stdout = ctypes.c_void_p.in_dll(libc, 'stdout')

@contextmanager
def stdout_redirector(stream) :
    # The original fd stdout points to.Usually 1 on POSIX systems.
    original_stdout_fd = sys.stdout.fileno()

    def _redirect_stdout(to_fd) :
        """Redirect stdout to the given file descriptor."""
        # Flush the C - level buffer stdout
        libc.fflush(c_stdout)
        # Flush and close sys.stdout - also closes the file descriptor(fd)
        sys.stdout.close()
        # Make original_stdout_fd point to the same file as to_fd
        os.dup2(to_fd, original_stdout_fd)
        # Create a new sys.stdout that points to the redirected fd
        sys.stdout = io.TextIOWrapper(os.fdopen(original_stdout_fd, 'wb'))

        # Save a copy of the original stdout fd in saved_stdout_fd
        saved_stdout_fd = os.dup(original_stdout_fd)
        try :
            # Create a temporary file and redirect stdout to it
            tfile = tempfile.TemporaryFile(mode = 'w+b')
            _redirect_stdout(tfile.fileno())
            # Yield to caller, then redirect stdout back to the saved fd
            yield
            _redirect_stdout(saved_stdout_fd)
            # Copy contents of temporary file to the given stream
            tfile.flush()
            tfile.seek(0, io.SEEK_SET)
            stream.write(tfile.read())
        finally:
            tfile.close()
            os.close(saved_stdout_fd)
)";



// See below for information on redirection of str3eams.
//http://eli.thegreenplace.net/2015/redirecting-all-kinds-of-stdout-in-python/

struct stdout_redirect {
	stdout_redirect(std::streambuf * new_buffer)
		: old(std::cout.rdbuf(new_buffer))
	{ }

	~stdout_redirect() {
		std::cout.rdbuf(old);
	}

private:
	std::streambuf * old;
};

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
		
		this->printCommandExecutionResults(coutstream.str().c_str(), QConsole::Complete);
	}
	catch (py::error_already_set &e) {
		this->printCommandExecutionResults(e.what(), QConsole::Error);
	}
}
