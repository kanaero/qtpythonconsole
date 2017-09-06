#include <qapplication.h>
#include <qpythonconsole.h>
#include <pybind11/embed.h>
namespace py = pybind11;

int main( int argc, char **argv )
{
    QApplication a( argc, argv );
	py::scoped_interpreter guard{}; // start the interpreter and keep it alive

	QPythonConsole console(NULL, "Welcome to Python 3.");
	console.resize(1204, 768);
	console.show();

    return a.exec();
}