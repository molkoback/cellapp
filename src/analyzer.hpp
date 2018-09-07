/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef ANALYZER_H
#define ANALYZER_H

#include "types.hpp"

#include <QString>
#include <QTextStream>

typedef struct _ContourParameters ContourParameters;

class Analyzer {
private:
	void process(CVContours &contours, std::vector<ContourParameters> &params);
	void writeResults(QTextStream &stream, std::vector<ContourParameters> &params);

public:
	Analyzer() {}
	~Analyzer() {}
	
	bool run(CVContours &contours, const QString &file);
};

#endif
