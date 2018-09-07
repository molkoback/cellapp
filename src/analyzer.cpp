/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "analyzer.hpp"

#include <opencv2/opencv.hpp>
#include <QFile>

#include <vector>
#include <cmath>

struct _ContourParameters {
	int x;        // x coordinate
	int y;        // y coordinate
	double perim; // Perimeter
	double area;  // Area
	float angle;  // Minimum rounding rectangle angle
	double dist;  // Distance from the largest cell
};

void Analyzer::process(CVContours &contours, std::vector<ContourParameters> &params)
{
	double largest_area = 0.0;
	int largest_x = 0;
	int largest_y = 0;
	for (const auto &cnt : contours) {
		ContourParameters p;
		
		cv::Moments moments = cv::moments(cnt);
		p.x = moments.m10 / moments.m00;
		p.y = moments.m01 / moments.m00;
		
		p.perim = cv::arcLength(cnt, true);
		p.area = cv::contourArea(cnt);
		if (p.area > largest_area) {
			largest_area = p.area;
			largest_x = p.x;
			largest_y = p.y;
		}
		
		cv::RotatedRect rect = cv::minAreaRect(cnt);
		p.angle = rect.angle;
		
		params.push_back(p);
	}
	
	for (auto &p : params) {
		double dx = p.x - largest_x;
		double dy = p.y - largest_y;
		p.dist = sqrt(dx*dx + dy*dy);
	}
}

void Analyzer::writeResults(QTextStream &stream, std::vector<ContourParameters> &params)
{
	stream << " \tArea\tPerim.\tAngle\tDist.\n";
	for (size_t i = 0; i < params.size(); i++) {
		ContourParameters &p = params[i];
		stream << i+1 << "\t" << p.area << "\t" << p.perim << "\t" << p.angle << "\t" << p.dist << "\n";
	}
}

bool Analyzer::run(CVContours &contours, const QString &file)
{
	QFile fp(file);
	if (!fp.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	QTextStream stream(&fp);
	
	std::vector<ContourParameters> params;
	this->process(contours, params);
	this->writeResults(stream, params);
	return true;
}
