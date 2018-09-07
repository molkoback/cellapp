/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef CELLLABEL_H
#define CELLLABEL_H

#include <QImage>
#include <QLabel>
#include <QMouseEvent>

class CellLabel : public QLabel {
	Q_OBJECT
signals:
	void clicked(int x, int y);

public:
	CellLabel() {}
	virtual ~CellLabel() {}
	
	void setImage(const QImage &im);
	void mousePressEvent(QMouseEvent *ev);
};

#endif
