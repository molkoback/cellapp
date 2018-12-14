/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef APP_H
#define APP_H

#include <QApplication>

#define APP_NAME "CellApp"
#define APP_ORG "CellApp"
#define APP_VERSION "0.4.0"

class App : public QApplication {
	Q_OBJECT
private:
public:
	App(int argc, char *argv[]);
	virtual ~App() {}
};

#endif
