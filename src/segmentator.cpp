/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "segmentator.hpp"

#include "util/convert.hpp"

#include <algorithm>
#include <exception>
#include <cstddef>

/* HSV threshold range. */
#define HSV_COLOR_LOW cv::Scalar(10, 0, 0)
#define HSV_COLOR_HIGH cv::Scalar(50, 192, 192)

/* Gaussian kernel size. */
#define GAUSSIAN_SIZE cv::Size(9, 9)

/* Contour draw colors. */
#define CONTOUR_DRAW_COLOR cv::Vec3b(0, 255, 0)

Segmentator::Segmentator() :
	filt_method(THRESHOLD_HSV),
	th_method(FILTER_BILATERAL),
	use_hull(0),
	prune_checks(CHECK_ALL),
	cell_minarea(25.0)
{}

void Segmentator::binarize(cv::UMat &src, cv::UMat &dst)
{
	cv::Mat mat;
	cv::UMat umat;
	
	switch (this->th_method) {
	case THRESHOLD_HSV:
		// NOTE: UMat here will crash on cleanup
		cv::cvtColor(src, mat, CV_BGR2HSV_FULL);
		cv::inRange(mat, HSV_COLOR_LOW, HSV_COLOR_HIGH, dst);
		break;
	case THRESHOLD_OTSU:
		cv::cvtColor(src, umat, CV_BGR2GRAY);
		cv::threshold(umat, dst, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
		break;
	default:
		throw std::runtime_error("invalid threshold method");
	}
}

void Segmentator::filter(cv::UMat &src, cv::UMat &dst)
{
	switch (this->filt_method) {
	case FILTER_NONE:
		src.copyTo(dst);
		break;
	case FILTER_BILATERAL:
		cv::bilateralFilter(src, dst, 9, 75, 75);
		break;
	case FILTER_GAUSSIAN:
		cv::GaussianBlur(src, dst, GAUSSIAN_SIZE, 75, 75);
		break;
	default:
		throw std::runtime_error("invalid filtering method");
	}
}

void Segmentator::segment(cv::UMat &im, CVContours &contours)
{
	cv::UMat im_filt, im_b;
	
	this->filter(im, im_filt);
	this->binarize(im_filt, im_b);
	
	std::vector<cv::Vec4i> hierarchy;
	if (this->use_hull) {
		// Find contours and then calculate their hulls
		CVContours tmp;
		cv::findContours(im_b, tmp, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
		for (const auto &cnt : tmp) {
			CVContour hull;
			cv::convexHull(cnt, hull);
			contours.push_back(hull);
		}
	}
	else {
		// Find contours
		cv::findContours(im_b, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	}
}

bool Segmentator::contourValid(const CVContour &cnt)
{
	bool ret = 1;
	if (this->prune_checks & CHECK_AREA)
		ret &= cv::contourArea(cnt) >= this->cell_minarea;
	return ret;
}

void Segmentator::pruneContours(CVContours &contours)
{
	contours.erase(
		std::remove_if(contours.begin(), contours.end(), [this] (CVContour &cnt) {
			return !this->contourValid(cnt);
		}),
		contours.end()
	);
}

void Segmentator::findContours(const QImage &src, CVContours &contours)
{
	cv::UMat mat;
	QImage2UMat(src, mat);
	this->segment(mat, contours);
	this->pruneContours(contours);
}

void Segmentator::drawContours(const QImage &src, QImage &dst, CVContours &contours)
{
	cv::UMat mat;
	QImage2UMat(src, mat);
	cv::drawContours(mat, contours, -1, CONTOUR_DRAW_COLOR, 2);
	UMat2QImage(mat, dst);
}

void Segmentator::removeContour(CVContours &contours, int x, int y)
{
	contours.erase(
		std::remove_if(contours.begin(), contours.end(), [x, y] (CVContour &cnt) {
			return cv::pointPolygonTest(cnt, cv::Point(x, y), false) >= 0;
		}),
		contours.end()
	);
}
