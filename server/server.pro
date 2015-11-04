QT += core
QT -= gui
QT += network

TARGET = server
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    users_db.cpp \
    serialization.cpp \
    balloons_generator.cpp \
    balloon.cpp

HEADERS += \
    server.hpp \
    users_db.hpp \
    balloon.hpp \
    balloons_generator.hpp \
    serialization.hpp

