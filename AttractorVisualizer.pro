#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T22:07:22
#
#-------------------------------------------------

QT       += core opengl gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AttractorVisualizer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/fileutills.cpp \
    src/Opengl/ptraj.cpp \
    src/Opengl/oglattractor.cpp \
    src/GUI/mainwindow.cpp \
    src/GUI/addtrajwindow.cpp \
    src/Opengl/camera.cpp

HEADERS += \
    src/fileutills.hpp \
    src/Opengl/ptraj.hpp \
    src/Opengl/oglattractor.hpp \
    src/GUI/mainwindow.hpp \
    src/GUI/addtrajwindow.hpp \
    src/Opengl/camera.hpp

DISTFILES += \
    src/Opengl/Shaders/fragment_shader.glsl \
    src/Opengl/Shaders/vertex_shader.glsl

RESOURCES += \
    shaders.qrc
