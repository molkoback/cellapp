/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "celllabel.hpp"

#include <iostream>

void CellLabel::setImage(const QImage &im)
{
	QPixmap pixmap = QPixmap::fromImage(im);
	this->setPixmap(pixmap);
	this->adjustSize();
}

void CellLabel::mousePressEvent(QMouseEvent *ev)
{
	emit clicked(ev->x(), ev->y());
}
