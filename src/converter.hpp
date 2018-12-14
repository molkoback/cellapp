/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is licensed under the terms of the MIT license. See LICENSE file
 * for more information.
 */

#ifndef CONVERTER_H
#define CONVERTER_H

#include <QImage>
#include <opencv2/opencv.hpp>

class Converter {
private:
	Converter() {}

public:
	static void UMat2QImage(const cv::UMat &src, QImage &dst);
	static void QImage2UMat(const QImage &src, cv::UMat &dst);
};

#endif
