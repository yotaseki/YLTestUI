project(YOLOTestUI)
cmake_minimum_required(VERSION 3.0)
set(DARKNET_DIR $ENV{DARKNET_DIR})
option(USE_GPU OFF)
find_package(OpenCV)
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(USE_GPU)
    add_definitions(-std=c++11 -DOPENCV -DGPU)
else(USE_GPU)
    add_definitions(-std=c++11 -DOPENCV)
endif(USE_GPU)
set(YOLOTestUI_SRCS
    ./main.cpp
    ./YOLOTestUI.cpp
    ./detect.cpp
    ./detect.h
    ./MyQclass.cpp
    ./MyQclass.h
    ./readtxt.cpp
    ./readtxt.h
    ./test.cpp
    ./test.h
    ./plotGraph.cpp
    ./plotGraph.h
	)
set(YOLOTestUI_MOC_HDRS
    ./YOLOTestUI.h
	)
set(YOLOTestUI_UIS
    ./YOLOTestUI.ui
    ./VisBboxUI.ui
	)

include_directories(
    ${DARKNET_DIR}/include
)

link_directories(
    ${DARKNET_DIR}
)

add_executable(YOLOTestUI
	${YOLOTestUI_SRCS}
	${YOLOTestUI_UIS}
	${YOLOTestUI_MOC_SRCS}
)

QT5_USE_MODULES(YOLOTestUI Widgets)
target_link_libraries(YOLOTestUI
	${OpenCV_LIBS}
    darknet
	)
