QT += core
QT -= gui

TARGET = BubbleKiller
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

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
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_contrib \
-lopencv_legacy \
-lopencv_flann \
-lopencv_videoio \

SOURCES += \
    src/main.cpp \
    src/balloons_generator.cpp \
    src/balloon.cpp \
    src/game.cpp \
    src/imageop.cpp \
    src/main1.cpp \
    src/webcam_image.cpp \
    src/rect.cpp \
    src/hand_detector.cpp

HEADERS += \
    include/balloons_generator.hpp \
    include/balloon.hpp \
    include/game.hpp \
    include/webcam_image.hpp \
    include/rect.hpp \
    include/imageop.hpp \
    include/hand_detector.hpp \
    src/game.cpp.autosave
