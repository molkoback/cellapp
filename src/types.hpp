/* Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef TYPES_HPP
#define TYPES_HPP

#include <opencv2/opencv.hpp>

typedef std::vector<cv::Point> CVContour;
typedef std::vector<CVContour> CVContours;

#endif
