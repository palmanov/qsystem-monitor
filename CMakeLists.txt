cmake_minimum_required(VERSION 3.15)

#
# Project details
#

project(
  "qsystem-monitor"
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(headers thirdparty/qroundprogressbar/qroundprogressbar.h 
            include/abstractmonitor.h
            include/cpumonitor.h
            include/cpuwidget.h
            include/loadavgmonitor.h
            include/rammonitor.h
            include/uptimemonitor.h
            include/ramwidget.h
            include/mainwindow.h)

set(sources thirdparty/qroundprogressbar/qroundprogressbar.cpp 
            src/cpumonitor.cpp
            src/cpuwidget.cpp
            src/loadavgmonitor.cpp
            src/rammonitor.cpp
            src/uptimemonitor.cpp
            src/mainwindow.cpp
            src/ramwidget.cpp
            src/main.cpp)

add_executable(${PROJECT_NAME} ${headers} ${sources})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/thirdparty)

# Add the Qt5 Widgets for linking
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)
