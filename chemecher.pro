# #####################################################################
# Automatically generated by qmake (2.01a) Fri Jun 4 11:29:03 2010
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    src \
    src/core \
    src/gui
INCLUDEPATH += . \
    src/gui \
    src/core
QT += sql \
    xml

# Input
HEADERS += src/core/cpd.h \
    src/core/mix.h \
    src/core/step.h \
    src/gui/cpdwindow.h \
    src/gui/mainwindow.h \
    src/gui/stepwindow.h \
    src/gui/mechdb.h \
    src/core/dbhandler.h \
    src/gui/sqlconfig.h \
    src/gui/simparams.h \
    src/core/iomanager.h \
    src/gui/about.h \
    src/gui/formatwizard.h
FORMS += src/gui/cpdwindow.ui \
    src/gui/mainwindow.ui \
    src/gui/stepwindow.ui \
    src/gui/mechdb.ui \
    src/gui/sqlconfig.ui \
    src/gui/simparams.ui \
    src/gui/about.ui \
    src/gui/formatwizard.ui
SOURCES += src/chemecher.cpp \
    src/core/cpd.cpp \
    src/core/mix.cpp \
    src/core/step.cpp \
    src/gui/cpdwindow.cpp \
    src/gui/mainwindow.cpp \
    src/gui/stepwindow.cpp \
    src/gui/mechdb.cpp \
    src/core/dbhandler.cpp \
    src/gui/sqlconfig.cpp \
    src/gui/simparams.cpp \
    src/core/iomanager.cpp \
    src/gui/about.cpp \
    src/gui/formatwizard.cpp
