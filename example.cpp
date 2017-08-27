#include <qapplication.h>
#include <qpushbutton.h>
#include <qpythonconsole.h>

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

	QPythonConsole console;
	console.show();

    return a.exec();
}