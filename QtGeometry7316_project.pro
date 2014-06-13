#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T00:23:22
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGeometry7316_project
TEMPLATE = app



INCLUDEPATH += $$PWD/graphics
DEPENDPATH += $$PWD/graphics

INCLUDEPATH += $$PWD/task
DEPENDPATH += $$PWD/task

SOURCES += main.cpp\
        mainwindow.cpp \
    graphics/glview.cpp \
    task/task.cpp

HEADERS  += mainwindow.h \
    graphics/glview.h \
    task/task.h \
    support/AVLTree.h \
    support/Node_Data.h

FORMS    += mainwindow.ui \
    mainwindow.ui

INCLUDEPATH += $$PWD/task/ \
               $$PWD/graphics/ \
               $$PWD/support/

DEPENDPATH  +=  $$PWD/task/ \
                $$PWD/graphics/ \
                $$PWD/support/

INCLUDEPATH += $$PWD/dependencies/GeoLib4/include/
DEPENDPATH  += $$PWD/dependencies/GeoLib4/include/

CONFIG(release,debug|release):LIBS+= -L$$PWD/dependencies/GeoLib4/lib -lGeolib -lGeoPolygons
CONFIG(debug,debug|release):LIBS+= -L$$PWD/dependencies/GeoLib4/lib -lGeolibd -lGeoPolygonsd





