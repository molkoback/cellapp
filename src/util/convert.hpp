/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef CONVERT_H
#define CONVERT_H

#include <QImage>
#include <opencv2/opencv.hpp>

void UMat2QImage(const cv::UMat &src, QImage &dst);

void QImage2UMat(const QImage &src, cv::UMat &dst);

#endif
