TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += $(OPENCV_BUILD)/

SOURCES += main.cpp

