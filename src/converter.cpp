/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
 */

#include "converter.hpp"

void Converter::UMat2QImage(const cv::UMat &src, QImage &dst)
{
	cv::Mat mat;
	cv::cvtColor(src, mat, cv::COLOR_BGR2RGB);
	QImage tmp(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
	dst = tmp.copy(); // Create deep copy
}

void Converter::QImage2UMat(const QImage &src, cv::UMat &dst)
{
	QImage im = src.convertToFormat(QImage::Format_RGB888);
	cv::Mat tmp(im.height(), im.width(), CV_8UC3, (void*)im.bits(), im.bytesPerLine());
	cv::UMat mat;
	tmp.copyTo(mat);
	cv::cvtColor(mat, dst, cv::COLOR_RGB2BGR);
}
