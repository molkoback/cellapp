/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "cellapp.hpp"
#include "cellmainwin.hpp"

int main(int argc, char *argv[])
{
	CellApp app(argc, argv);
	CellMainWin win;
	win.show();
	return app.exec();
}
