#include "qt_python_console.h"
#include <QColor>
#include <pybind11/embed.h>
#include <iostream>
#include <strstream>
#include <sstream>
#include <pybind11/iostream.h>

namespace py = pybind11;

QPythonConsole::QPythonConsole(QWidget *parent, const QString &welcomeText) : QConsole(parent, welcomeText), Python3Console() 
{
}

QPythonConsole::~QPythonConsole() {

}


void QPythonConsole::Initialize()
{
	connect(
		this, &QConsole::execCommand,
		this, &QPythonConsole::ExecuteAndPrintResults
	);
	QFont f("unexistent");
	f.setStyleHint(QFont::Monospace);
	this->setFont(f);
	Python3Console::Initialize();

}

void QPythonConsole::InsertText(CursorLocation where, std::string& text)
{

}

void QPythonConsole::MoveCursor(CursorLocation where)
{

}

void QPythonConsole::DisplayPrompt()
{
	this->displayPrompt();

}

void QPythonConsole::Print(const std::string &command, SuccessMode mode)
{

}

void QPythonConsole::ExecuteAndPrintResults(const QString &command) {
	QByteArray ba = command.toLocal8Bit();
	this->ExecAndPrintCommand(ba.data());
}
