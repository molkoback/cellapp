/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
 */

#include "mainwin.hpp"

#include "app.hpp"

#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QKeySequence>
#include <QList>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QScrollArea>
#include <QStatusBar>
#include <QString>
#include <QStringList>

MainWin::MainWin() :
	m_options(new OptionsWin())
{
	// Load window options
	restoreGeometry(m_options->geometry());
	m_inputPath = m_options->inputPath();
	
	// Load segmentator options
	loadOptions();
	
	createMenuBar();
	createView();
	
	// Options changed
	connect(m_options, SIGNAL(saved()), this, SLOT(on_optionsChanged()));
	
	// Clicks from imLabel
	connect(&m_imLabel, SIGNAL(clicked(int, int)), this, SLOT(on_imLabelClicked(int, int)));
}

/* Close event that will be called when the windows is being closed.*/
void MainWin::closeEvent(QCloseEvent *event)
{
	m_options->setGeometry(saveGeometry());
	m_options->setInputPath(m_inputPath);
	m_options->save();
	QMainWindow::closeEvent(event);
}

void MainWin::loadOptions()
{
	struct segm_param param = m_options->segmParam();
	m_segmentator.setFiltMethod(param.filtMethod);
	m_segmentator.setThMethod(param.thMethod);
	m_segmentator.setUseWatershed(param.useWatershed);
	m_segmentator.setCellShape(param.cellShape);
	m_segmentator.setCellMinArea(param.cellMinArea);
}

void MainWin::createMenuBar()
{
	QMenuBar *menu = menuBar();
	
	// File menu
	QMenu *fileMenu = menu->addMenu("File");
	
	QAction *openFileAction = fileMenu->addAction("Open File");
	openFileAction->setShortcut(QKeySequence("Ctrl+O"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(on_openFileAction()));
	
	QAction *openDirAction = fileMenu->addAction("Open Folder");
	openDirAction->setShortcut(QKeySequence("Ctrl+Shift+O"));
	connect(openDirAction, SIGNAL(triggered()), this, SLOT(on_openDirAction()));
	
	m_saveImageAction = fileMenu->addAction("Save Image");
	m_saveImageAction->setShortcut(QKeySequence("Ctrl+S"));
	connect(m_saveImageAction, SIGNAL(triggered()), this, SLOT(on_saveImageAction()));
	m_saveImageAction->setEnabled(false);
	
	m_saveResultsAction = fileMenu->addAction("Save Results");
	m_saveResultsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
	connect(m_saveResultsAction, SIGNAL(triggered()), this, SLOT(on_saveResultsAction()));
	m_saveResultsAction->setEnabled(false);
	
	fileMenu->addSeparator();
	
	QAction *quitAction = fileMenu->addAction("Quit");
	quitAction->setShortcut(QKeySequence("Ctrl+Q"));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(on_quitAction()));
	
	// Tools menu
	QMenu *toolsMenu = menu->addMenu("Tools");
	
	m_processAction = toolsMenu->addAction("Process");
	m_processAction->setShortcut(QKeySequence("Ctrl+P"));
	connect(m_processAction, SIGNAL(triggered()), this, SLOT(on_processAction()));
	m_processAction->setEnabled(false);
	
	m_processAllAction = toolsMenu->addAction("Process All");
	m_processAllAction->setShortcut(QKeySequence("Ctrl+Shift+P"));
	connect(m_processAllAction, SIGNAL(triggered()), this, SLOT(on_processAllAction()));
	m_processAllAction->setEnabled(false);
	
	toolsMenu->addSeparator();
	
	QAction *optionsAction = toolsMenu->addAction("Options");
	connect(optionsAction, SIGNAL(triggered()), this, SLOT(on_optionsAction()));
	
	// Help menu
	QMenu *helpMenu = menu->addMenu("Help");
	
	QAction *aboutAction = helpMenu->addAction(QString("About %1").arg(APP_NAME));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(on_aboutAction()));
	
	QAction *aboutQtAction = helpMenu->addAction("About Qt");
	connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(on_aboutQtAction()));
}

void MainWin::createView()
{
	QPointer<QScrollArea> scrollArea = new QScrollArea(this);
	setCentralWidget(scrollArea);
	scrollArea->setWidget(&m_imLabel);
}

/* Loads a the given image. */
bool MainWin::loadImage(const QString &file)
{
	m_imOrig = QImage(file);
	if (m_imOrig.isNull()) {
		QMessageBox::information(
			this, QGuiApplication::applicationDisplayName(),
			QString("Couldn't open '%1'").arg(file)
		);
		return false;
	}
	
	// Set our image
	m_imLabel.setImage(m_imOrig);
	m_contours.clear();
	
	// Enable processing
	m_processAction->setEnabled(true);
	m_processAllAction->setEnabled(true);
	
	// Disable saving
	m_saveImageAction->setEnabled(false);
	m_saveResultsAction->setEnabled(false);
	return true;
}

void MainWin::statusMessage(const QString &msg)
{
	statusBar()->showMessage(msg, 0);
}

void MainWin::on_openFileAction()
{
	QString file = QFileDialog::getOpenFileName(
		this,
		"Open Image", m_inputPath,
		"Image Files (*.png *.jpg *.bmp *.gif)"
	);
	if (file.isEmpty())
		return;
	m_inputPath = QFileInfo(file).absolutePath();
	
	m_files.clear();
	m_files << file;
	statusMessage(QString("Opened %1").arg(file));
	loadImage(file);
}

void MainWin::on_openDirAction()
{
	QString path = QFileDialog::getExistingDirectory(
		this,
		"Open Folder", m_inputPath
	);
	if (path.isEmpty())
		return;
	m_inputPath = path;
	
	QDir dir(path);
	QList<QFileInfo> fileinfos = dir.entryInfoList(
		QStringList() << "*.png" <<  "*.jpg" <<  "*.bmp" << "*.gif"
	);
	
	m_files.clear();
	for (const auto &fileinfo : fileinfos)
		m_files << fileinfo.absoluteFilePath();
	statusMessage(QString("Opened %1 files").arg(m_files.size()));
	loadImage(m_files[0]);
}

void MainWin::on_saveImageAction()
{
	QString file = QFileDialog::getSaveFileName(
		this,
		"Save Image", m_inputPath,
		"Image Files (*.png *.jpg *.bmp *.gif)"
	);
	if (!file.isEmpty()) {
		m_imSegm.save(file);
		statusMessage(QString("Saved %1").arg(file));
	}
}

void MainWin::on_saveResultsAction()
{
	QString file = QFileDialog::getSaveFileName(
		this,
		"Save File", m_inputPath,
		"Text Files (*.txt)"
	);
	if (!file.isEmpty()) {
		m_analyzer.run(m_contours, file);
		statusMessage(QString("Saved %1").arg(file));
	}
}

void MainWin::on_quitAction()
{
	close();
}

void MainWin::on_processAction()
{
	// Segment
	m_segmentator.segment(m_imOrig, m_contours);
	m_segmentator.drawContours(m_imOrig, m_imSegm, m_contours);
	m_imLabel.setImage(m_imSegm);
	
	// Enable file saving
	m_saveImageAction->setEnabled(true);
	m_saveResultsAction->setEnabled(true);
	statusMessage(QString("Processed %1").arg(m_files[0]));
}

void MainWin::on_processAllAction()
{
	QString outputPath = m_options->outputPath();
	size_t n = m_files.size();
	for (size_t i = 0; i < n; i++) {
		statusMessage(QString("Processing file %1/%2").arg(i+1).arg(n));
		
		// Load image
		loadImage(m_files[i]);
		QCoreApplication::processEvents();
		
		// Segment
		m_segmentator.segment(m_imOrig, m_contours);
		m_segmentator.drawContours(m_imOrig, m_imSegm, m_contours);
		m_imLabel.setImage(m_imSegm);
		QCoreApplication::processEvents();
		
		QFileInfo info(m_files[i]);
		
		// Save results
		if (m_options->saveResults()) {
			QString txtfile = outputPath + "/" + info.baseName() + QString(".txt");
			m_analyzer.run(m_contours, txtfile);
		}
		
		// Save image
		if (m_options->saveImage()) {
			QString imfile = outputPath + "/" + info.baseName() + QString("_segm.png");
			m_imSegm.save(imfile);
		}
	}
	statusMessage(QString("Processed %1 files").arg(n));
}

void MainWin::on_optionsAction()
{
	m_options->show();
}

void MainWin::on_aboutAction()
{
	QString msg = QString(
		"%1 v%2<br>"
		"<br>"
		"Copyright (C) 2018 <a href=\"mailto:eero.molkoselka@gmail.com\">Eero Molkoselkä</a><br>"
		"<br>"
		"This work is licensed under the terms of the MIT license. See LICENSE file for more information.<br>"
	).arg(APP_NAME, APP_VERSION);
	QMessageBox::about(this, QString("About %1").arg(APP_NAME), msg);
}

void MainWin::on_aboutQtAction()
{
	QMessageBox::aboutQt(this);
}

void MainWin::on_optionsChanged()
{
	m_contours.clear();
	loadOptions();
}

void MainWin::on_imLabelClicked(int x, int y)
{
	if (!m_contours.empty()) {
		m_segmentator.removeContour(m_contours, x, y);
		m_segmentator.drawContours(m_imOrig, m_imSegm, m_contours);
		m_imLabel.setImage(m_imSegm);
	}
}
