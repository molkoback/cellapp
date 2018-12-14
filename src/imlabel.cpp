/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "imlabel.hpp"

#include <iostream>

void ImLabel::setImage(const QImage &im)
{
	QPixmap pixmap = QPixmap::fromImage(im);
	setPixmap(pixmap);
	adjustSize();
}

void ImLabel::mousePressEvent(QMouseEvent *ev)
{
	emit clicked(ev->x(), ev->y());
}
