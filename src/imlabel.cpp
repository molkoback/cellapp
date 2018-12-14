/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
 */

#include "imlabel.hpp"

#include <QApplication>

ImLabel::ImLabel() :
	m_zoom(1.0)
{}

void ImLabel::update()
{
	QPixmap pixmap = QPixmap::fromImage(m_im).scaled(
		m_im.width()*m_zoom, m_im.height()*m_zoom,
		Qt::KeepAspectRatio,
		Qt::FastTransformation
	);
	this->setPixmap(pixmap);
	this->adjustSize();
}

void ImLabel::setImage(const QImage &im)
{
	m_im = im;
	update();
}

const QImage &ImLabel::image()
{
	return m_im;
}

void ImLabel::mousePressEvent(QMouseEvent *ev)
{
	emit clicked(ev->x()/m_zoom, ev->y()/m_zoom);
}

void ImLabel::wheelEvent(QWheelEvent *e)
{
	if (m_im.isNull())
		return;
	if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
		m_zoom = e->angleDelta().y() > 0 ? m_zoom*1.25 : m_zoom/1.25;
		update();
	}
}
