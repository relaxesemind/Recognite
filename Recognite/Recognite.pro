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



FORMS += \
        mainwindow.ui \
    diagramwindow.ui \
    parsersettingsdialog.ui \
    traversewalksettings.ui

RESOURCES += \
    resources/resources.qrc

HEADERS += \
    Common/consts.h \
    Common/currentappstate.h \
    Common/magic.h \
    Common/settingsstorage.h \
    Controllers/diagramwindow.h \
    Controllers/mainwindow.h \
    Controllers/parsersettingsdialog.h \
    Controllers/traversewalksettings.h \
    Managers/parser.h \
    Models/area.h \
    Models/inputmodel.h \
    Models/models.h \
    Views/GradientAxis/gradientaxis.h \
    Views/ImageView/imageview.h \
    Views/ImageView/imageviewmode.h \
    core.h \
    grapher.h \
    selectingprocessmanager.h \
    staticmodel.h \
    Managers/selectingprocessmanager.h \
    Models/seriamodel.h \
    Common/colorgenerator.h

SOURCES += \
    Common/currentappstate.cpp \
    Common/settingsstorage.cpp \
    Controllers/diagramwindow.cpp \
    Controllers/mainwindow.cpp \
    Controllers/parsersettingsdialog.cpp \
    Controllers/traversewalksettings.cpp \
    Managers/parser.cpp \
    Models/area.cpp \
    Models/inputmodel.cpp \
    Views/GradientAxis/gradientaxis.cpp \
    Views/ImageView/imageview.cpp \
    core.cpp \
    grapher.cpp \
    main.cpp \
    staticmodel.cpp \
    Managers/selectingprocessmanager.cpp \
    Models/seriamodel.cpp \
    Common/colorgenerator.cpp
