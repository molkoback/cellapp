/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
 */

#ifndef ANALYZER_H
#define ANALYZER_H

#include "types.hpp"

#include <QString>
#include <QTextStream>

typedef struct _ContourParameters ContourParameters;

class Analyzer {
private:
	void contourXY(const CVContour &cnt, int &x, int &y);
	double contourRatio(const CVContour &cnt);
	void process(CVContours &contours, std::vector<ContourParameters> &params);
	void writeResults(QTextStream &stream, std::vector<ContourParameters> &params);

public:
	Analyzer() {}
	~Analyzer() {}
	
	bool run(CVContours &contours, const QString &file);
};

#endif
