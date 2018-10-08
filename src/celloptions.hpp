/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef CELLOPTIONS_H
#define CELLOPTIONS_H

#include "cellmainwin.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QString>
#include <QVBoxLayout>

class CellMainWin;

class CellOptions : public QDialog {
	Q_OBJECT
private:
	QSettings settings;
	
	QByteArray geometry;
	QString inputPath;
	QString outputPath;
	
	QVBoxLayout vbox;
	
	QGroupBox segmBox;
	QFormLayout segmLayout;
	QLabel filtMethodLabel;
	QComboBox filtMethodComboBox;
	QLabel thMethodLabel;
	QComboBox thMethodComboBox;
	QLabel watershedLabel;
	QCheckBox watershedCheckBox;
	QLabel hullLabel;
	QCheckBox hullCheckBox;
	
	QGroupBox cellBox;
	QFormLayout cellLayout;
	QLabel minareaLabel;
	QLineEdit minareaLineEdit;
	
	QGroupBox outputBox;
	QFormLayout outputLayout;
	QLabel saveResultsLabel;
	QCheckBox saveResultsCheckBox;
	QLabel saveImageLabel;
	QCheckBox saveImageCheckBox;
	QLabel outputPathLabel;
	QPushButton outputPathButton;
	
	QHBoxLayout buttonLayout;
	QPushButton okButton;
	QPushButton cancelButton;
	
	bool settingsValid();

signals:
	void changed();

public slots:
	void on_outputPathReleased();
	void on_okReleased();
	void on_cancelReleased();

public:
	CellOptions();
	virtual ~CellOptions() {}
	
	void loadSettings();
	void saveSettings();
	
	void setGeometry(const QByteArray &geometry);
	void setInputPath(const QString &path) { this->inputPath = path; }
	
	QByteArray getGeometry();
	QString getInputPath() { return this->inputPath; }
	QString getOutputPath() { return this->outputPath; }
	int getFiltMethod();
	int getThMethod();
	bool getUseWatershed();
	bool getUseHull();
	double getCellMinArea();
	bool getSaveResults();
	bool getSaveImage();
};

#endif
