QT += core
QT -= gui

TARGET = BubbleKiller
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ./include
INCLUDEPATH += ./src

INCLUDEPATH += /usr/local/include/opencv

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_imgcodecs \
-lopencv_videoio \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_flann \

SOURCES += \
    src/main.cpp \
    src/balloons_generator.cpp \
    src/hand_detector.cpp \
    src/webcam_image.cpp \
    src/imageop.cpp \
    src/rect.cpp

HEADERS += \
    include/balloons_generator.hpp \
    include/hand_detector.hpp \
    include/webcam_image.hpp \
    include/imageop.hpp \
    include/rect.hpp
