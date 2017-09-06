#ifndef NODELAB_PYTHON3_CONSOLE_H
#define NODELAB_PYTHON3_CONSOLE_H

#include <string>

class Python3Console;
class Python3Console {
public:
	Python3Console();
	virtual ~Python3Console();
	static Python3Console& Get();
	void Reset() {};
	void Clear() {};
	void SetNormalPrompt() { prompt = ">>> "; }
	void SetMultilinePrompt() { prompt = "... "; }
	void ReadKeyboard();
	std::string& GetCurrentCommand() { return command; }

	// Interpreter related methods.
	virtual void Initialize();
	virtual void Shutdown();
	virtual void InterpretShortCutCommands(const std::string &command);
	virtual void PrintHistory();
	virtual void LoadScript(const std::string& fn) {}
	virtual void SaveScript(const std::string& fn) {}
	virtual void ExecAndPrintCommand(const std::string &command);


	//// Display related methods.
	enum class CursorLocation { END, CURRENT };
	enum class SuccessMode { Successful, Error };
	virtual void Append(std::string text) { printf("%s", text.c_str()); }
	virtual void InsertText(CursorLocation where, std::string& text) { printf("%s", text.c_str()); }
	virtual void MoveCursor(CursorLocation where) {}
	virtual void DisplayPrompt();
	virtual void Print(const std::string &command, SuccessMode mode);


	// Some internal variables.
	int lines{ 0 };
	std::string prompt;
	bool read_keyboard{ true };
	std::string line_buffer; // accumulates characters typed in one at at time.
	std::string command;

};

#endif //NODELAB_PYTHON_INTERPRETER_H
