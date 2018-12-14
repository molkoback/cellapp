/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "segmentator.hpp"

#include "converter.hpp"

#include <algorithm>
#include <exception>
#include <cstddef>

/* HSV threshold range. */
#define HSV_COLOR_LOW cv::Scalar(10, 0, 0)
#define HSV_COLOR_HIGH cv::Scalar(50, 192, 192)

/* Gaussian kernel size. */
#define GAUSSIAN_SIZE cv::Size(9, 9)

/* Watershed options. */
#define WS_KERNEL_SIZE cv::Size(5, 5)
#define WS_NOPEN 2
#define WS_NDILATE 6

/* Contour draw colors. */
#define CONTOUR_DRAW_COLOR cv::Vec3b(0, 255, 0)

Segmentator::Segmentator() :
	m_filtMethod(THRESHOLD_HSV),
	m_thMethod(FILTER_BILATERAL),
	m_useWatershed(0),
	m_useHull(0),
	m_pruneChecks(CHECK_ALL),
	m_cellMinArea(25.0)
{}

void Segmentator::binarize(cv::UMat &src, cv::UMat &dst)
{
	cv::Mat mat;
	cv::UMat umat;
	
	switch (m_thMethod) {
	case THRESHOLD_HSV:
		// NOTE: UMat here will crash on cleanup
		cv::cvtColor(src, mat, cv::COLOR_BGR2HSV_FULL);
		cv::inRange(mat, HSV_COLOR_LOW, HSV_COLOR_HIGH, dst);
		break;
	case THRESHOLD_OTSU:
		cv::cvtColor(src, umat, cv::COLOR_BGR2GRAY);
		cv::threshold(umat, dst, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
		break;
	default:
		throw std::runtime_error("invalid threshold method");
	}
}

void Segmentator::filter(cv::UMat &src, cv::UMat &dst)
{
	switch (m_filtMethod) {
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

void Segmentator::watershed(cv::UMat &im, cv::UMat &im_b, cv::UMat &dst)
{
	cv::UMat fg, bg, unknown, markers, mask;
	
	// Find sure foreground and background
	cv::Mat kernel = getStructuringElement(cv::MORPH_ELLIPSE, WS_KERNEL_SIZE);
	cv::morphologyEx(im_b, fg, cv::MORPH_OPEN, kernel, cv::Point(-1,-1), WS_NOPEN);
	cv::morphologyEx(fg, bg, cv::MORPH_DILATE, kernel, cv::Point(-1,-1), WS_NDILATE);
	cv::subtract(bg, fg, unknown);
	
	// Find markers
	cv::connectedComponents(fg, markers);
	cv::add(markers, 1, markers); // Background will be 1
	
	// Mark unknown region
	cv::compare(unknown, 255, mask, cv::CMP_EQ);
	markers.setTo(0, mask);
	
	// Watershed
	cv::watershed(im, markers);
	
	// Binarize marker image correctly
	// Set background to 0
	cv::compare(markers, 1, mask, cv::CMP_EQ);
	markers.setTo(0, mask);
	// Set everything else to 255
	cv::compare(markers, 0, mask, cv::CMP_NE);
	markers.setTo(255, mask);
	markers.convertTo(markers, CV_8U);
	
	// Remove 1 pixel border left by watershed
	cv::UMat area(markers, cv::Rect(1, 1, markers.cols-2, markers.rows-2));
	cv::UMat cropped = area.clone();
	cv::copyMakeBorder(cropped, dst, 1, 1, 1, 1, cv::BORDER_REPLICATE);
}

void Segmentator::findContours(cv::UMat &im_b, CVContours &contours)
{
	std::vector<cv::Vec4i> hierarchy;
	if (m_useHull) {
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
	if (m_pruneChecks & CHECK_AREA)
		ret &= cv::contourArea(cnt) >= m_cellMinArea;
	return ret;
}

void Segmentator::pruneContours(CVContours &contours)
{
	contours.erase(
		std::remove_if(contours.begin(), contours.end(), [this] (CVContour &cnt) {
			return !contourValid(cnt);
		}),
		contours.end()
	);
}

void Segmentator::segment(const QImage &src, CVContours &contours)
{
	cv::UMat im, im_filt, im_b, im_b_ws;
	
	Converter::QImage2UMat(src, im);
	filter(im, im_filt);
	binarize(im_filt, im_b);
	if (m_useWatershed) {
		watershed(im, im_b, im_b_ws);
		im_b = im_b_ws;
	}
	findContours(im_b, contours);
	pruneContours(contours);
}

void Segmentator::drawContours(const QImage &src, QImage &dst, CVContours &contours)
{
	cv::UMat mat;
	Converter::QImage2UMat(src, mat);
	cv::drawContours(mat, contours, -1, CONTOUR_DRAW_COLOR, 2);
	Converter::UMat2QImage(mat, dst);
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
