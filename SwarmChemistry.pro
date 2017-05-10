QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SwarmChemistry
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -fopenmp
#LIBS += -lgomp -lpthread
LIBS += -lomp

SOURCES += main.cpp\
        mainwindow.cpp \
    openglwindow.cpp \
    swarm2d.cpp \
    swarm2dwindow.cpp \
    swarmmodel.cpp \
    particle.cpp \
    swarm.cpp \
    swarmrelation.cpp

HEADERS  += mainwindow.h \
    openglwindow.h \
    swarm2d.h \
    swarm2dwindow.h \
    swarmmodel.h \
    particle.h \
    swarm.h \
    swarmrelation.h

DISTFILES += \
    common.frag \
    swarm2d.frag \
    common.vert \
    swarm2d.vert
