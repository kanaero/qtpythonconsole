#include "python3_console.h"
#include <pybind11/embed.h>
#include <string>
#include <memory>
#include <fcntl.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <iostream>
#include <pybind11/iostream.h>


#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

namespace py = pybind11;

static std::string out_text;

class Redirector {
public:
	Redirector() {}
	virtual ~Redirector() {}
	void write(std::string text) {
		::out_text.append(text);
	}

	void Clear() {
		out_text.clear();
	}
	static Redirector& Get() {
		static Redirector global_instance;
		return global_instance;
	}
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


PYBIND11_EMBEDDED_MODULE(redirector, m) {
	m.doc() = "pybind11 stdout/stderr redirection module";
	py::class_<Redirector>(m, "Redirector")
		.def("write", &Redirector::write)
		.def_static("Get", &Redirector::Get);
}

Python3Console::Python3Console() : lines(0) {
}

Python3Console::~Python3Console() {
	this->Shutdown();
}


void Python3Console::Initialize()
{
	SetNormalPrompt();
	DisplayPrompt();

	try {
		py::exec("import redirector");
		py::exec("import sys");
		py::exec("sys.stdout = redirector.Redirector.Get()");
		py::exec("sys.stderr = redirector.Redirector.Get()");
	}
	catch (py::error_already_set &e) {
		this->Print(e.what(), SuccessMode::Error);
	}
}

void Python3Console::Shutdown()
{

}

Python3Console& Python3Console::Get() {
	static Python3Console global_instance;
	return global_instance;
}


void Python3Console::InterpretShortCutCommands(const std::string &cmd) {
#if 0
	std::string command = cmd;
	std::string result;
	char buf[4 * 1024];
	if (cmd == "\n") {
		result = "";
	}
	else if (cmd == "pwd") {
		this->ExecCommand("os.getcwd()", true, false, NULL, false);
	}
	else if (cmd == "cdh") {
		this->ExecCommand("NL_DIR = os.getenv('NL_DIR')", true, false, NULL, false);
		this->ExecCommand("homedir=os.path.join(NL_DIR,'nodelab')", true, false, NULL, false);
		this->ExecCommand("os.chdir(homedir)");
	}
	else if (StartsWith(cmd, "cd")) {
		std::vector<std::string> parts = Split(Trim(cmd), " ");
		if (parts.size() >= 2) {
			sprintf(buf, "os.chdir(\"%s\")", parts[1].c_str());
			this->ExecCommand(buf, true, false, NULL, false);
		}
		else {
			result = "usage cd <dir>";
		}
	}
	else if (StartsWith(cmd, "run")) {
		std::vector<std::string> parts = Split(Trim(cmd), " ");
		if (parts.size() >= 2) {
			std::string fn = parts.at(1) + ".py";
			std::ifstream file(fn);
			if (!file) {
				result = fn + " not found";
			}
			else {
				while (file) {
					std::string line;
					getline(file, line);
					this->ExecCommand(line, true, false, &result, false);
				}
			}
			file.close();
		}
		else {
			this->Append("usage: run <filename> without py extension");
		}
	}
	else {
		this->InterpretCommand(cmd);
	}
#endif
	this->ExecAndPrintCommand(cmd);
}

void Python3Console::Print(const std::string &text, SuccessMode mode) {
	if (mode == SuccessMode::Successful) {
		printf("\n%s", text.c_str());
	}
	else if (mode == SuccessMode::Error) {
		printf("\n%s", text.c_str());
	}
	this->DisplayPrompt();
}

void Python3Console::ExecAndPrintCommand(const std::string &command) {
	std::stringbuf  coutstream;
	std::stringbuf  cerrstream;

	cout_redirect cout_guard(&coutstream);
	cerr_redirect cerr_guard(&cerrstream);
	//this->RedirectStdout();
	//printf("this is stdout output\n");
	//py::scoped_ostream_redirect output;

	try {
		if (command.size() > 0) {
			auto result = py::eval<py::eval_single_statement>(command.c_str());
		}
		else {
			auto result = py::eval<py::eval_single_statement>(command.c_str());
		}
		this->Print(out_text, SuccessMode::Successful);
		Redirector::Get().Clear();
	}
	catch (py::error_already_set &e) {
		this->Print(e.what(), SuccessMode::Error);
		Redirector::Get().Clear();
	}
	//this->RestoreStdout();
}

void Python3Console::DisplayPrompt() {
	printf("%s", current_prompt.c_str());
}
void Python3Console::PrintHistory() {
	printf("%s", "Print History Called");
}

void Python3Console::ReadKeyboard() {
#ifdef WIN32
	while (_kbhit()) {
		unsigned char ch = (unsigned char)_getche();
		if (ch == '\n' || ch == '\r') {
			this->ExecAndPrintCommand(line_buffer);
			line_buffer.clear();
		}
		else {
			line_buffer.push_back(ch);
		}
	}
#else
	char ch; int ret = 0;
	fcntl(0, F_SETFL, O_NONBLOCK);
	do {
		ret = read(0, &ch, 1);
		if (ret > 0) {
			if (ch == '\n') {
				this->ExecCommand(line_buffer, false, true, NULL, false);
				line_buffer.clear();
			}
			else {
				line_buffer.push_back(ch);
			}
		}
	} while (ret > 0);
#endif
}

void Python3Console::RedirectStdout()
{

	//https://stackoverflow.com/questions/955962/how-to-buffer-stdout-in-memory-and-write-it-from-a-dedicated-thread
	//https://www.thecodingforums.com/threads/redirect-stdout-to-a-buffer.705392/
	// 
#if defined(_WIN32)
	//CreatePipe( &hReadPipe, &hWritePipe, NULL, 1024);
	//SetStdHandle(STD_OUTPUT_HANDLE, hWritePipe);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	// Create a new console window.
	if (!AllocConsole()) return;

	// Set the screen buffer to be larger than normal (this is optional).
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
	{
		csbi.dwSize.Y = 1000; // any useful number of lines...
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), csbi.dwSize);
	}


	// Redirect "stdin" to the console window.
	if (!freopen("CONIN$", "w", stdin)) return;


	// Redirect "stderr" to the console window.
	if (!freopen("CONOUT$", "w", stderr)) return;

	// Redirect "stdout" to the console window.
	if (!freopen("CONOUT$", "w", stdout)) return;


	// Turn off buffering for "stdout" ("stderr" is unbuffered by default).

	setbuf(stdout, NULL);

#endif
}

void Python3Console::RestoreStdout()
{
#if defined(_WIN32)
	//CloseHandle(hWritePipe);
	//ReadFile(hReadPipe, stdout_buffer, 19, &nr, NULL);
	//CloseHandle(hReadPipe);
	//WriteFile(GetStdHandle(STD_ERROR_HANDLE), stdout_buffer, nr, &nw, NULL);
#endif
}


