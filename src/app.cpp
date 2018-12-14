/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "app.hpp"

App::App(int argc, char *argv[]) :
	QApplication(argc, argv)
{
	QApplication::setOrganizationName(APP_ORG);
	QApplication::setApplicationName(APP_NAME);
	QApplication::setApplicationVersion(APP_VERSION);
}
