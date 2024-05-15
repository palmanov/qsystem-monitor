cmake_minimum_required(VERSION 3.15)

project(
  "qsystem-monitor"
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(headers include/info/abstractsysinfo.h
            include/info/cpuinfo.h
            include/info/loadavginfo.h
            include/info/memoryinfo.h
            include/info/uptimeinfo.h
            include/cpuwidget.h
            include/mainwindow.h
            include/memorywidget.h
            include/utils.h)

set(sources src/info/cpuinfo.cpp
            src/info/loadavginfo.cpp
            src/info/memoryinfo.cpp
            src/info/uptimeinfo.cpp
            src/cpuwidget.cpp
            src/main.cpp
            src/mainwindow.cpp
            src/memorywidget.cpp)

add_executable(${PROJECT_NAME} ${headers} ${sources})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/thirdparty)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)
