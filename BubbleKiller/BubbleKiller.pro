QT += core
QT -= gui

TARGET = BubbleKiller
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11
CONFIG += -Wno-sign-compare

CONFIG(release, debug|release) {
    DESTDIR = release
} else {
    DESTDIR = debug
}

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
    src/game.cpp \
    src/balloon.cpp \
    src/hand_detector.cpp \
    src/webcam_image.cpp \
    src/rect.cpp \
    src/image_utils.cpp

HEADERS += \
    include/balloons_generator.hpp \
    include/hand_detector.hpp \
    include/balloon.hpp \
    include/game.hpp \
    include/webcam_image.hpp \
    include/rect.hpp \
    include/image_utils.hpp

copyfiles.commands = cp -r ../$${TARGET}/images $${DESTDIR}/

RESOURCES += \
    images.qrc

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
