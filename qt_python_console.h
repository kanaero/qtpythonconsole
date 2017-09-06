#pragma once

#include "qconsole.h"

class QPythonConsole : public QConsole {
	Q_OBJECT;
public:
	QPythonConsole(QWidget *parent = NULL, const QString &welcomeText = "");
	virtual ~QPythonConsole();

	public Q_SLOTS:
		void ExecuteAndPrintResults(const QString &command);

};