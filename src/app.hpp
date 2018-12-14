/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
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
