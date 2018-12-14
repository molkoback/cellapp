/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef CELLOPTIONS_H
#define CELLOPTIONS_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QSettings>
#include <QString>

struct segm_param {
	int filtMethod;
	int thMethod;
	bool useWatershed;
	int cellShape;
	int cellMinArea;
};

class OptionsWin : public QDialog {
	Q_OBJECT
private:
	QSettings m_settings;
	
	QByteArray m_geometry;
	QString m_inputPath;
	QString m_outputPath;
	
	QComboBox m_filtMethodComboBox;
	QComboBox m_thMethodComboBox;
	QCheckBox m_watershedCheckBox;
	QComboBox m_cellShapeComboBox;
	QLineEdit m_minareaLineEdit;
	QCheckBox m_saveResultsCheckBox;
	QCheckBox m_saveImageCheckBox;

private:
	bool settingsValid();

public:
	OptionsWin();
	virtual ~OptionsWin() {}
	
	void load();
	void save();
	
	void setGeometry(const QByteArray &geometry);
	void setInputPath(const QString &path) { m_inputPath = path; }
	
	QByteArray geometry();
	QString inputPath() { return m_inputPath; }
	QString outputPath() { return m_outputPath; }
	struct segm_param segmParam();
	bool saveResults();
	bool saveImage();

signals:
	void loaded();
	void saved();

public slots:
	void on_outputPathReleased();
	void on_okReleased();
	void on_cancelReleased();
};

#endif
