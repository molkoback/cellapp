/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "celloptions.hpp"

#include "cellapp.hpp"
#include "segmentator.hpp"

#include <QMessageBox>

CellOptions::CellOptions() :
	settings(CELLAPP_NAME, CELLAPP_ORG),
	segmBox("Segmentation"),
	filtMethodLabel("Filtering method"),
	thMethodLabel("Threshold method"),
	hullLabel("Use convex hull"),
	cellBox("Cells"),
	minareaLabel("Minimum cell area (px)"),
	okButton("OK"),
	cancelButton("Cancel")
{
	this->setLayout(&this->vbox);
	
	// Segmentation
	this->vbox.addWidget(&this->segmBox);
	this->segmBox.setLayout(&this->segmLayout);
	
	this->segmLayout.addRow(&this->filtMethodLabel, &this->filtMethodComboBox);
	this->filtMethodComboBox.addItem("None");
	this->filtMethodComboBox.addItem("Gaussian");
	this->filtMethodComboBox.addItem("Bilateral");
	
	this->segmLayout.addRow(&this->thMethodLabel, &this->thMethodComboBox);
	this->thMethodComboBox.addItem("Color based");
	this->thMethodComboBox.addItem("Otsu's method");
	
	this->segmLayout.addRow(&this->hullLabel, &this->hullCheckBox);
	
	// Cells
	this->vbox.addWidget(&this->cellBox);
	this->cellBox.setLayout(&this->cellLayout);
	this->cellLayout.addRow(&this->minareaLabel, &this->minareaLineEdit);
	
	this->vbox.addStretch();
	
	this->vbox.addLayout(&this->buttonLayout);
	this->buttonLayout.addWidget(&this->okButton);
	connect(&this->okButton, SIGNAL(released()), this, SLOT(on_okReleased()));
	this->buttonLayout.addWidget(&this->cancelButton);
	connect(&this->cancelButton, SIGNAL(released()), this, SLOT(on_cancelReleased()));
	
	this->loadSettings();
}

void CellOptions::loadSettings()
{
	this->geometry = this->settings.value("mainwin/geometry").toByteArray();
	this->currentPath = this->settings.value("files/currentPath", ".").toString();
	this->filtMethodComboBox.setCurrentIndex(this->settings.value("segm/filt_method", FILTER_BILATERAL).toInt());
	this->hullCheckBox.setChecked(this->settings.value("segm/use_hull", false).toBool());
	this->thMethodComboBox.setCurrentIndex(this->settings.value("segm/th_method", THRESHOLD_HSV).toInt());
	this->minareaLineEdit.setText(this->settings.value("cell/minarea", 25.0).toString());
}

void CellOptions::saveSettings()
{
	this->settings.setValue("mainwin/geometry", this->geometry);
	this->settings.setValue("files/currentPath", this->currentPath);
	this->settings.setValue("segm/filt_method", this->filtMethodComboBox.currentIndex());
	this->settings.setValue("segm/th_method", this->thMethodComboBox.currentIndex());
	this->settings.setValue("segm/use_hull", this->hullCheckBox.isChecked());
	this->settings.setValue("cell/minarea", this->minareaLineEdit.text().toDouble());
	this->settings.sync();
}

bool CellOptions::settingsValid()
{
	bool ok;
	this->minareaLineEdit.text().toDouble(&ok);
	if (!ok) return false;
	return true;
}

void CellOptions::on_okReleased()
{
	if (this->settingsValid()) {
		this->saveSettings();
		emit changed();
		this->hide();
	}
	else {
		QMessageBox::information(
			this, QGuiApplication::applicationDisplayName(),
			QString("Invalid settings given")
		);
	}
}

void CellOptions::on_cancelReleased()
{
	this->loadSettings();
	this->hide();
}

void CellOptions::setGeometry(const QByteArray &geometry)
{
	this->geometry = geometry;
}

void CellOptions::setCurrentPath(const QString &path)
{
	this->currentPath = path;
}

QByteArray CellOptions::getGeometry()
{
	return this->geometry;
}

QString CellOptions::getCurrentPath()
{
	return this->currentPath;
}

int CellOptions::getFiltMethod()
{
	return this->filtMethodComboBox.currentIndex();
}

int CellOptions::getThMethod()
{
	return this->thMethodComboBox.currentIndex();
}

bool CellOptions::getUseHull()
{
	return this->hullCheckBox.isChecked();
}

double CellOptions::getCellMinArea()
{
	return this->minareaLineEdit.text().toDouble();
}
