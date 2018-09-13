/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef CELLAPP_H
#define CELLAPP_H

#include <QApplication>

#define CELLAPP_NAME "CellApp"
#define CELLAPP_ORG "CellApp"
#define CELLAPP_VERSION "0.3.0"

class CellApp : public QApplication {
	Q_OBJECT
private:
public:
	CellApp(int argc, char *argv[]);
	virtual ~CellApp() {}
};

#endif
