/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
 */

#include "optionswin.hpp"

#include "app.hpp"
#include "segmentator.hpp"

#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPointer>
#include <QPushButton>
#include <QVBoxLayout>

OptionsWin::OptionsWin() :
	m_settings(APP_NAME, APP_ORG)
{
	QPointer<QVBoxLayout> vbox = new QVBoxLayout();
	setLayout(vbox);
	
	// Segmentation
	QPointer<QGroupBox> segmBox = new QGroupBox("Segmentation", this);
	vbox->addWidget(segmBox);
	QPointer<QFormLayout> segmLayout = new QFormLayout();
	segmBox->setLayout(segmLayout);
	
	QPointer<QLabel> filtMethodLabel = new QLabel("Filtering method", this);
	segmLayout->addRow(filtMethodLabel, &m_filtMethodComboBox);
	m_filtMethodComboBox.addItem("None");
	m_filtMethodComboBox.addItem("Gaussian");
	m_filtMethodComboBox.addItem("Bilateral");
	QPointer<QLabel> thMethodLabel = new QLabel("Threshold method", this);
	segmLayout->addRow(thMethodLabel, &m_thMethodComboBox);
	m_thMethodComboBox.addItem("Color based");
	m_thMethodComboBox.addItem("Otsu's method");
	QPointer<QLabel> watershedLabel = new QLabel("Use watershed", this);
	segmLayout->addRow(watershedLabel, &m_watershedCheckBox);
	
	// Cells
	QPointer<QGroupBox> cellBox = new QGroupBox("Cells", this);
	vbox->addWidget(cellBox);
	QPointer<QFormLayout> cellLayout = new QFormLayout();
	cellBox->setLayout(cellLayout);
	QPointer<QLabel> cellShapeLabel = new QLabel("Shape accuracy", this);
	cellLayout->addRow(cellShapeLabel, &m_cellShapeComboBox);
	m_cellShapeComboBox.addItem("Accurate");
	m_cellShapeComboBox.addItem("Approximation");
	m_cellShapeComboBox.addItem("Hull");
	QPointer<QLabel> minAreaLabel = new QLabel("Minimum area (px)", this);
	cellLayout->addRow(minAreaLabel, &m_minareaLineEdit);
	
	// Output files
	QPointer<QGroupBox> outputBox = new QGroupBox("Output files", this);
	vbox->addWidget(outputBox);
	QPointer<QFormLayout> outputLayout = new QFormLayout();
	outputBox->setLayout(outputLayout);
	QPointer<QLabel> saveResultsLabel = new QLabel("Save analysis results", this);
	outputLayout->addRow(saveResultsLabel, &m_saveResultsCheckBox);
	QPointer<QLabel> saveImageLabel = new QLabel("Save segmented image", this);
	outputLayout->addRow(saveImageLabel, &m_saveImageCheckBox);
	QPointer<QLabel> outputPathLabel = new QLabel("Output folder", this);
	QPointer<QPushButton> outputPathButton = new QPushButton("Browse...", this);
	outputLayout->addRow(outputPathLabel, outputPathButton);
	connect(outputPathButton, SIGNAL(released()), this, SLOT(on_outputPathReleased()));
	
	vbox->addStretch();
	
	QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
	vbox->addLayout(buttonLayout);
	QPointer<QPushButton> okButton = new QPushButton("OK", this);
	buttonLayout->addWidget(okButton);
	connect(okButton, SIGNAL(released()), this, SLOT(on_okReleased()));
	QPointer<QPushButton> cancelButton = new QPushButton("Cancel", this);
	buttonLayout->addWidget(cancelButton);
	connect(cancelButton, SIGNAL(released()), this, SLOT(on_cancelReleased()));
	
	load();
}

void OptionsWin::load()
{
	m_geometry = m_settings.value("mainwin/geometry").toByteArray();
	m_inputPath = m_settings.value("files/inputPath", ".").toString();
	m_outputPath = m_settings.value("files/outputPath", ".").toString();
	m_filtMethodComboBox.setCurrentIndex(m_settings.value("segm/filt_method", FILTER_BILATERAL).toInt());
	m_watershedCheckBox.setChecked(m_settings.value("segm/use_watershed", false).toBool());
	m_cellShapeComboBox.setCurrentIndex(m_settings.value("segm/cell_shape", SHAPE_ACCURATE).toInt());
	m_thMethodComboBox.setCurrentIndex(m_settings.value("segm/th_method", THRESHOLD_HSV).toInt());
	m_minareaLineEdit.setText(m_settings.value("cell/minarea", 25.0).toString());
	m_saveResultsCheckBox.setChecked(m_settings.value("output/save_results", true).toBool());
	m_saveImageCheckBox.setChecked(m_settings.value("output/save_image", false).toBool());
	emit loaded();
}

void OptionsWin::save()
{
	m_settings.setValue("mainwin/geometry", m_geometry);
	m_settings.setValue("files/inputPath", m_inputPath);
	m_settings.setValue("files/outputPath", m_outputPath);
	m_settings.setValue("segm/filt_method", m_filtMethodComboBox.currentIndex());
	m_settings.setValue("segm/th_method", m_thMethodComboBox.currentIndex());
	m_settings.setValue("segm/use_watershed", m_watershedCheckBox.isChecked());
	m_settings.setValue("segm/cell_shape", m_cellShapeComboBox.currentIndex());
	m_settings.setValue("cell/minarea", m_minareaLineEdit.text().toDouble());
	m_settings.setValue("output/save_results", m_saveResultsCheckBox.isChecked());
	m_settings.setValue("output/save_image", m_saveImageCheckBox.isChecked());
	m_settings.sync();
	emit saved();
}

bool OptionsWin::settingsValid()
{
	bool ok;
	m_minareaLineEdit.text().toDouble(&ok);
	if (!ok) return false;
	return true;
}

void OptionsWin::on_outputPathReleased()
{
	QString path = QFileDialog::getExistingDirectory(
		this,
		"Open Folder", m_outputPath
	);
	if (!path.isEmpty())
		m_outputPath = path;
}

void OptionsWin::on_okReleased()
{
	if (settingsValid()) {
		save();
		hide();
	}
	else {
		QMessageBox::information(
			this, QGuiApplication::applicationDisplayName(),
			QString("Invalid settings given")
		);
	}
}

void OptionsWin::on_cancelReleased()
{
	load();
	hide();
}

void OptionsWin::setGeometry(const QByteArray &geometry) { m_geometry = geometry; }
QByteArray OptionsWin::geometry() { return m_geometry; }

struct segm_param OptionsWin::segmParam()
{
	segm_param param;
	param.filtMethod = m_filtMethodComboBox.currentIndex();
	param.thMethod = m_thMethodComboBox.currentIndex();
	param.useWatershed = m_watershedCheckBox.isChecked();
	param.cellShape = m_cellShapeComboBox.currentIndex();
	param.cellMinArea = m_minareaLineEdit.text().toDouble();
	return param;
}

bool OptionsWin::saveResults() { return m_saveResultsCheckBox.isChecked(); }
bool OptionsWin::saveImage() { return m_saveImageCheckBox.isChecked(); }
