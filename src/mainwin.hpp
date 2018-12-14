/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
 */

#ifndef CELLMAINWIN_H
#define CELLMAINWIN_H

#include "optionswin.hpp"
#include "imlabel.hpp"
#include "types.hpp"
#include "segmentator.hpp"
#include "analyzer.hpp"

#include <QMainWindow>
#include <QLabel>
#include <QPointer>

class MainWin : public QMainWindow {
	Q_OBJECT
private:
	QPointer<OptionsWin> m_options;
	
	QImage m_imOrig;
	QImage m_imSegm;
	ImLabel m_imLabel;
	
	Segmentator m_segmentator;
	CVContours m_contours;
	Analyzer m_analyzer;
	
	QAction *m_saveImageAction;
	QAction *m_saveResultsAction;
	QAction *m_processAction;
	QAction *m_processAllAction;
	
	QString m_inputPath;
	QStringList m_files;

private:
	/* Close event that will be called when the windows is being closed.*/
	void closeEvent(QCloseEvent *event);
	
	void loadOptions();
	void createMenuBar();
	void createView();
	
	/* Loads a the given image. */
	bool loadImage(const QString &file);
	
	void statusMessage(const QString &msg);

public:
	MainWin();
	virtual ~MainWin() {}

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
	void on_imLabelClicked(int x, int y);
};

#endif
