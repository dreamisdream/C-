#-------------------------------------------------
#
# Project created by QtCreator 2020-05-26T11:49:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BeyondAbyss
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
        main.cpp \
        beyondabysswidget.cpp \
    customstatusbar.cpp \
    updateinfoshowwidget.cpp \
    loginmainwindow.cpp \
    registermainwindow.cpp \
    client.cpp \
    fileinfo.cpp \
    filelistwidget.cpp \
    mylineedit.cpp \
    mybutton.cpp \
    filetransfromsocket.cpp \
    recyclebin.cpp \
    recyclebinfileinfo.cpp \
    logwidget.cpp \
    limitmanagementdelegate.cpp \
    limitsmanagement.cpp \
    sharefiledialog.cpp \
    downloadwidget.cpp \
    sharefilewidget.cpp

HEADERS += \
        beyondabysswidget.h \
    customstatusbar.h \
    updateinfoshowwidget.h \
    loginmainwindow.h \
    registermainwindow.h \
    client.h \
    data.h \
    fileinfo.h \
    fileinfo.h \
    filelistwidget.h \
    mylineedit.h \
    mybutton.h \
    filetransfromsocket.h \
    recyclebin.h \
    recyclebinfileinfo.h \
    logwidget.h \
    limitmanagementdelegate.h \
    limitsmanagement.h \
    sharefiledialog.h \
    downloadwidget.h \
    sharefilewidget.h

FORMS += \
        beyondabysswidget.ui \
    loginmainwindow.ui \
    registermainwindow.ui \
    downloadwidget.ui \
    sharefiledialog.ui

RESOURCES += \
    image.qrc \
    photo01.qrc

DISTFILES +=
