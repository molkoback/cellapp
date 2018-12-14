/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
 */

#include "app.hpp"

App::App(int argc, char *argv[]) :
	QApplication(argc, argv)
{
	QApplication::setOrganizationName(APP_ORG);
	QApplication::setApplicationName(APP_NAME);
	QApplication::setApplicationVersion(APP_VERSION);
}
