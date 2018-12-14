/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef SEGMENTATOR_H
#define SEGMENTATOR_H

#include "types.hpp"

#include <opencv2/opencv.hpp>
#include <QImage>

#include <vector>

enum threshold_methods {
	THRESHOLD_HSV = 0,
	THRESHOLD_OTSU
};

enum filtering_methods {
	FILTER_NONE = 0,
	FILTER_GAUSSIAN,
	FILTER_BILATERAL
};

enum prune_check_methods {
	CHECK_NONE = 0,
	CHECK_AREA = 1,
	CHECK_ALL = 1
};

class Segmentator {
private:
	int m_filtMethod;
	int m_thMethod;
	
	bool m_useWatershed;
	bool m_useHull;
	
	int m_pruneChecks;
	double m_cellMinArea;
	
	void binarize(cv::UMat &src, cv::UMat &dst);
	void filter(cv::UMat &src, cv::UMat &dst);
	void watershed(cv::UMat &im, cv::UMat &im_b, cv::UMat &dst);
	void findContours(cv::UMat &im_b, CVContours &contours);
	bool contourValid(const CVContour &cnt);
	void pruneContours(CVContours &contours);

public:
	Segmentator();
	~Segmentator() {}
	
	void setFiltMethod(int filtMethod) { m_filtMethod = filtMethod; }
	void setThMethod(int thMethod) { m_thMethod = thMethod; }
	void setUseWatershed(bool useWatershed) { m_useWatershed = useWatershed; }
	void setUseHull(bool useHull) { m_useHull = useHull; }
	void setPruneChecks(int pruneChecks) { m_pruneChecks = pruneChecks; }
	void setCellMinArea(double cellMinArea) { m_cellMinArea = cellMinArea; }
	
	void segment(const QImage &src, CVContours &contours);
	void drawContours(const QImage &src, QImage &dst, CVContours &contours);
	void removeContour(CVContours &contours, int x, int y);
};

#endif
