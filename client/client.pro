QT += core
QT -= gui
QT += network

TARGET = client
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

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

SOURCES += main.cpp \
    client.cpp \
    serialization.cpp \
    balloon.cpp \
    balloons_generator.cpp \
    cascade_detector.cpp \
    game.cpp \
    hand_detector.cpp \
    image_utils.cpp \
    rect.cpp \
    webcam_image.cpp

HEADERS += \
    client.h \
    serialization.hpp \
    balloon.hpp \
    balloons_generator.hpp \
    cascade_detector.hpp \
    game.hpp \
    hand_detector.hpp \
    image_utils.hpp \
    rect.hpp \
    webcam_image.hpp

copyfiles.commands = cp -r ../images .

RESOURCES += \
    images.qrc

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
