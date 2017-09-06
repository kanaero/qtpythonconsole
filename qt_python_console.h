#pragma once

#include "qt_console.h"
#include "python3_console.h"
class QPythonConsole : public QConsole, public Python3Console {
	Q_OBJECT;
public:
	QPythonConsole(QWidget *parent = NULL, const QString &welcomeText = "");
	virtual ~QPythonConsole();

	//// Display related methods.
	//virtual void Append(std::string text) override;
	virtual void Initialize() override;
	virtual void InsertText(CursorLocation where, std::string& text) override;
	virtual void MoveCursor(CursorLocation where) override;
	virtual void DisplayPrompt() override;
	virtual void Print(const std::string &command, SuccessMode mode) override;


	public Q_SLOTS:
		void ExecuteAndPrintResults(const QString &command);

};