# Copyright (c) 2018 Eero Molkoselkä <eero.molkoselka@gmail.com>
# 
# This work is free. You can redistribute it and/or modify it under the
# terms of the Do What The Fuck You Want To Public License, Version 2,
# as published by Sam Hocevar. See the COPYING file for more details.
#

cmake_minimum_required(VERSION 3.9)

project(cell_project)

set(CMAKE_AUTOMOC ON)


# Sources
set(CELL_SRC
	src/analyzer.cpp
	src/main.cpp
	src/cellapp.cpp
	src/celllabel.cpp
	src/cellmainwin.cpp
	src/celloptions.cpp
	src/segmentator.cpp
	src/util/convert.cpp
)
set(CELL_NAME "cellapp")

set(EXECUTABLE_OUTPUT_PATH "bin")
if(${WIN32} AND ${CMAKE_BUILD_TYPE} STREQUAL "Release")
	add_executable(${CELL_NAME} WIN32 ${CELL_SRC})
else()
	add_executable(${CELL_NAME} ${CELL_SRC})
endif()


# CXXFlags
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++14 -Wall -Wextra -pipe")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS} -Og -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -march=native -flto")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -O3 -march=native -flto -DDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -Os -march=native -flto")


# Libs
find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED Core Widgets)
target_link_libraries(cellapp
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	${OpenCV_LIBS}
)
