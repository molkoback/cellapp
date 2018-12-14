/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef IMLABEL_H
#define IMLABEL_H

#include <QImage>
#include <QLabel>
#include <QWheelEvent>

class ImLabel : public QLabel {
	Q_OBJECT
private:
	float m_zoom;
	QImage m_im;

private:
	void update();

protected:
	void mousePressEvent(QMouseEvent *ev);
	void wheelEvent(QWheelEvent *e);

public:
	ImLabel();
	virtual ~ImLabel() {}
	
	void setImage(const QImage &im);
	const QImage &image();

signals:
	void clicked(int x, int y);
};

#endif
