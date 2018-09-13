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
	int filt_method;
	int th_method;
	
	bool use_watershed;
	bool use_hull;
	
	int prune_checks;
	double cell_minarea;
	
	void binarize(cv::UMat &src, cv::UMat &dst);
	void filter(cv::UMat &src, cv::UMat &dst);
	void watershed(cv::UMat &im, cv::UMat &im_b, cv::UMat &dst);
	void findContours(cv::UMat &im_b, CVContours &contours);
	void segment(cv::UMat &im, CVContours &contours);
	bool contourValid(const CVContour &cnt);
	void pruneContours(CVContours &contours);
	void findParameters(cv::UMat &im, CVContours &contours, struct SegmentResults &res);

public:
	Segmentator();
	~Segmentator() {}
	
	void setFiltMethod(int filt_method) { this->filt_method = filt_method; }
	void setThMethod(int th_method) { this->th_method = th_method; }
	void setUseWatershed(bool use_watershed) { this->use_watershed = use_watershed; }
	void setUseHull(bool use_hull) { this->use_hull = use_hull; }
	void setPruneChecks(int prune_checks) { this->prune_checks = prune_checks; }
	void setCellMinArea(double cell_minarea) { this->cell_minarea = cell_minarea; }
	
	void findContours(const QImage &src, CVContours &contours);
	void drawContours(const QImage &src, QImage &dst, CVContours &contours);
	void removeContour(CVContours &contours, int x, int y);
};

#endif
