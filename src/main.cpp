/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
 */

#include "app.hpp"
#include "mainwin.hpp"

int main(int argc, char *argv[])
{
	App app(argc, argv);
	MainWin win;
	win.show();
	return app.exec();
}
