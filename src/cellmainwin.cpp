/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "cellmainwin.hpp"

#include "cellapp.hpp"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QFileInfoList>
#include <QKeySequence>
#include <QList>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QStatusBar>
#include <QString>
#include <QStringList>

void CellMainWin::loadSegmOptions()
{
	CellOptions *opt = this->optionsWin.get();
	
	this->segmentator.setFiltMethod(opt->getFiltMethod());
	this->segmentator.setThMethod(opt->getThMethod());
	this->segmentator.setUseHull(opt->getUseHull());
	this->segmentator.setCellMinArea(opt->getCellMinArea());
}

void CellMainWin::saveWinOptions()
{
	CellOptions *opt = this->optionsWin.get();
	
	opt->setGeometry(this->saveGeometry());
	opt->setCurrentPath(this->currentPath);
	
	opt->saveSettings();
}

/* Close event that will be called when the windows is being closed.*/
void CellMainWin::closeEvent(QCloseEvent *event)
{
	this->saveWinOptions();
	QMainWindow::closeEvent(event);
}

void CellMainWin::createMenuBar()
{
	QMenuBar *menuBar = this->menuBar();
	
	// File menu
	QMenu *fileMenu = menuBar->addMenu("File");
	
	QAction *openFileAction = fileMenu->addAction("Open File");
	openFileAction->setShortcut(QKeySequence("Ctrl+O"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(on_openFileAction()));
	
	QAction *openDirAction = fileMenu->addAction("Open Folder");
	openDirAction->setShortcut(QKeySequence("Ctrl+Shift+O"));
	connect(openDirAction, SIGNAL(triggered()), this, SLOT(on_openDirAction()));
	openDirAction->setEnabled(false);
	
	this->saveImageAction = fileMenu->addAction("Save Image");
	this->saveImageAction->setShortcut(QKeySequence("Ctrl+S"));
	connect(this->saveImageAction, SIGNAL(triggered()), this, SLOT(on_saveImageAction()));
	this->saveImageAction->setEnabled(false);
	
	this->saveResultsAction = fileMenu->addAction("Save Results");
	this->saveResultsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
	connect(this->saveResultsAction, SIGNAL(triggered()), this, SLOT(on_saveResultsAction()));
	this->saveResultsAction->setEnabled(false);
	
	fileMenu->addSeparator();
	
	QAction *quitAction = fileMenu->addAction("Quit");
	quitAction->setShortcut(QKeySequence("Ctrl+Q"));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(on_quitAction()));
	
	// Tools menu
	QMenu *toolsMenu = menuBar->addMenu("Tools");
	
	this->processAction = toolsMenu->addAction("Process");
	this->processAction->setShortcut(QKeySequence("Ctrl+P"));
	connect(this->processAction, SIGNAL(triggered()), this, SLOT(on_processAction()));
	this->processAction->setEnabled(false);
	
	toolsMenu->addSeparator();
	
	QAction *optionsAction = toolsMenu->addAction("Options");
	connect(optionsAction, SIGNAL(triggered()), this, SLOT(on_optionsAction()));
	
	// Help menu
	QMenu *helpMenu = menuBar->addMenu("Help");
	
	QAction *aboutAction = helpMenu->addAction("About");
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(on_aboutAction()));
}

void CellMainWin::createView()
{
	this->setCentralWidget(&this->scrollArea);
	this->scrollArea.setWidget(&this->imageLabel);
}

CellMainWin::CellMainWin() :
	optionsWin(new CellOptions())
{
	// Load window options
	CellOptions *opt = this->optionsWin.get();
	this->restoreGeometry(opt->getGeometry());
	this->currentPath = opt->getCurrentPath();
	
	// Load segmentator options
	this->loadSegmOptions();
	
	this->createMenuBar();
	this->createView();
	
	// Options changed
	connect(optionsWin.get(), SIGNAL(changed()), this, SLOT(on_optionsChanged()));
	
	// Clicks from imageLabel
	connect(&imageLabel, SIGNAL(clicked(int, int)), this, SLOT(on_imageLabelClicked(int, int)));
}

/* Opens and sets our image. */
void CellMainWin::loadImage(const QString &file)
{
	this->image_orig = QImage(file);
	if (this->image_orig.isNull()) {
		QMessageBox::information(
			this, QGuiApplication::applicationDisplayName(),
			QString("Couldn't open '%1'").arg(file)
		);
		return;
	}
	
	// Set our image
	this->imageLabel.setImage(this->image_orig);
	this->contours.clear();
	
	// Enable processing
	this->processAction->setEnabled(true);
	
	// Disable saving
	this->saveImageAction->setEnabled(false);
	this->saveResultsAction->setEnabled(false);
}

void CellMainWin::on_openFileAction()
{
	QDir dir;
	QString file = QFileDialog::getOpenFileName(
		this,
		"Open Image", this->currentPath,
		"Image Files (*.png *.jpg *.bmp *.gif)"
	);
	if (file.isEmpty())
		return;
	this->currentPath = dir.absoluteFilePath(file);
	
	this->loadImage(file);
}

void CellMainWin::on_openDirAction()
{
	QString path = QFileDialog::getExistingDirectory();
	if (path.isEmpty())
		return;
	this->currentPath = path;
	
	QDir dir(path);
	QList<QFileInfo> fileinfos = dir.entryInfoList(
		QStringList() << "*.png" <<  "*.jpg" <<  "*.bmp" << "*.gif"
	);
	
	this->files.clear();
	for (const auto &fileinfo : fileinfos)
		this->files << fileinfo.absoluteFilePath();
}

void CellMainWin::on_saveImageAction()
{
	QString file = QFileDialog::getSaveFileName(
		this,
		"Save Image", this->currentPath,
		"Image Files (*.png *.jpg *.bmp *.gif)"
	);
	if (!file.isEmpty())
		this->image_segm.save(file);
}

void CellMainWin::on_saveResultsAction()
{
	QString file = QFileDialog::getSaveFileName(
		this,
		"Save File", this->currentPath,
		"Text Files (*.txt)"
	);
	if (!file.isEmpty()) {
		this->analyzer.run(this->contours, file);
	}
}

void CellMainWin::on_quitAction()
{
	this->close();
}

void CellMainWin::on_processAction()
{
	this->segmentator.findContours(this->image_orig, this->contours);
	this->segmentator.drawContours(this->image_orig, this->image_segm, this->contours);
	this->imageLabel.setImage(this->image_segm);
	
	// Enable file saving
	this->saveImageAction->setEnabled(true);
	this->saveResultsAction->setEnabled(true);
}

void CellMainWin::on_optionsAction()
{
	this->optionsWin.get()->show();
}

void CellMainWin::on_aboutAction()
{
	QString msg = QString(
		"%1 v%2<br>"
		"<br>"
		"Copyright (C) 2018 <a href=\"mailto:eero.molkoselka@gmail.com\">Eero Molkoselkä</a><br>"
		"<br>"
		"This software is licensed under WTFPL. See COPYING file for details.<br>"
	).arg(CELLAPP_NAME, CELLAPP_VERSION);
	QMessageBox::about(this, QString("About %1").arg(CELLAPP_NAME), msg);
}

void CellMainWin::on_optionsChanged()
{
	this->contours.clear();
	this->loadSegmOptions();
}

void CellMainWin::on_imageLabelClicked(int x, int y)
{
	if (!this->contours.empty()) {
		this->segmentator.removeContour(this->contours, x, y);
		this->segmentator.drawContours(this->image_orig, this->image_segm, this->contours);
		this->imageLabel.setImage(this->image_segm);
	}
}
