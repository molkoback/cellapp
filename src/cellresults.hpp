/* 
 * Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef CELLRESULTS_H
#define CELLRESULTS_H

#include <QDialog>

class CellResults : public QDialog {
	Q_OBJECT
private:
public:
	CellResults();
	virtual ~CellResults() {}
};

#endif
