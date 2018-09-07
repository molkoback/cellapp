/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "cellapp.hpp"

CellApp::CellApp(int argc, char *argv[]) :
	QApplication(argc, argv)
{
	QApplication::setOrganizationName(CELLAPP_ORG);
	QApplication::setApplicationName(CELLAPP_NAME);
	QApplication::setApplicationVersion(CELLAPP_VERSION);
}
