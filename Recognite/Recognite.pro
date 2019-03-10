#-------------------------------------------------
#
# Project created by QtCreator 2019-02-11T17:08:07
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#CONFIG += console

VERSION = 1.0.0.1
QMAKE_TARGET_PRODUCT = RecogniteApp
QMAKE_TARGET_COPYRIGHT = Egor Ivanov
QMAKE_TARGET_DESCRIPTION = SelectionObject

CONFIG += c++14

TARGET = Recognite
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

RC_ICONS = $${_PRO_FILE_PWD_}/resources/icon1.ico

#INCLUDEPATH += open_cv/include

#LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_calib3d320.dll
#LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_core320.dll
#LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_features2d320.dll
#LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_highgui320.dll
#LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_imgcodecs320.dll
#LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_photo320.dll
#LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_imgproc320.dll


#LIBS += -lopencv_core341 -lopencv_imgproc341 -lopencv_highgui341 \
#-lopencv_ml341 -lopencv_video341 -lopencv_features2d341 -lopencv_calib3d341 \
#-lopencv_objdetect341 -lopencv_flann341 -lopencv_videoio341


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    core.cpp \
    Models/area.cpp \
    Managers/parser.cpp \
    grapher.cpp \
    diagramwindow.cpp \
    Models/inputmodel.cpp \
    imageview.cpp \
    selectingprocessmanager.cpp \
    staticmodel.cpp \
    gradientaxis.cpp \
    parsersettingsdialog.cpp \
    traversewalksettings.cpp

HEADERS += \
        mainwindow.h \
    Common/magic.h \
    core.h \
    Models/area.h \
    Managers/parser.h \
    grapher.h \
    diagramwindow.h \
    Models/inputmodel.h \
    imageview.h \
    Models/models.h \
    selectingprocessmanager.h \
    Common/consts.h \
    staticmodel.h \
    gradientaxis.h \
    parsersettingsdialog.h \
    traversewalksettings.h

FORMS += \
        mainwindow.ui \
    diagramwindow.ui \
    parsersettingsdialog.ui \
    traversewalksettings.ui

RESOURCES += \
    resources/resources.qrc
