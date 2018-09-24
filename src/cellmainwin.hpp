/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef CELLMAINWIN_H
#define CELLMAINWIN_H

#include "celloptions.hpp"
#include "celllabel.hpp"
#include "types.hpp"
#include "segmentator.hpp"
#include "analyzer.hpp"

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>

#include <memory>

class CellOptions;

class CellMainWin : public QMainWindow {
	Q_OBJECT
private:
	std::unique_ptr<CellOptions> optionsWin;
	
	QScrollArea scrollArea;
	QImage image_orig;
	QImage image_segm;
	CellLabel imageLabel;
	
	Segmentator segmentator;
	CVContours contours;
	Analyzer analyzer;
	
	QAction *saveImageAction;
	QAction *saveResultsAction;
	QAction *processAction;
	QAction *processAllAction;
	
	QString currentPath;
	QStringList files;

private:
	/* Close event that will be called when the windows is being closed.*/
	void closeEvent(QCloseEvent *event);
	
	void createMenuBar();
	void createView();
	
	void loadSegmOptions();
	void saveWinOptions();
	
	/* Loads a the given image. */
	bool loadImage(const QString &file);
	
	void statusMessage(const QString &msg);

public slots:
	void on_openFileAction();
	void on_openDirAction();
	void on_saveImageAction();
	void on_saveResultsAction();
	void on_quitAction();
	void on_processAction();
	void on_processAllAction();
	void on_optionsAction();
	void on_aboutAction();
	void on_aboutQtAction();
	void on_optionsChanged();
	void on_imageLabelClicked(int x, int y);

public:
	CellMainWin();
	virtual ~CellMainWin() {}
};

#endif
