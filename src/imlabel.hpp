/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
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
