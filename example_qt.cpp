#include <qapplication.h>
#include <pybind11/embed.h>
#include "qt_python_console.h"
namespace py = pybind11;

int main( int argc, char **argv )
{
    QApplication a( argc, argv );
	py::scoped_interpreter guard{}; // start the interpreter and keep it alive

	QPythonConsole console(NULL, "Welcome to Python 3.");
	console.Initialize();
	console.resize(1204, 768);
	console.show();

    return a.exec();
}