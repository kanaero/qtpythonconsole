#include "python3_console.h"
#include <pybind11/embed.h>

int main( int argc, char **argv )
{
	pybind11::scoped_interpreter guard{}; // start the interpreter and keep it alive

	Python3Console console;
	console.Initialize();
	while (true) {
		console.ReadKeyboard();
	}
}