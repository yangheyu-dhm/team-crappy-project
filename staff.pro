#-------------------------------------------------
#
# Project created by QtCreator 2025-07-10T10:55:51
#
#-------------------------------------------------

QT       += core gui

QT       += sql
QT += sql charts


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = staff
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    staffmainwindow.cpp \
    repairservicewidget.cpp \
    devicemonitorwidget.cpp \
    parkingmanagerwidget.cpp \
    leaveapplicationwidget.cpp \
    activitymanagerwidget.cpp \
    raddwidget.cpp \
    daddwidget.cpp \
    paddwidget.cpp \
    aaddwidget.cpp \
    laddwidget.cpp \
    loginpage.cpp \
    database.cpp \
    accesswidget.cpp \
    adminwindow.cpp \
    attendancewidget.cpp \
    carmanagewidget.cpp \
    leaveapprovewidget.cpp \
    parkingcarin.cpp \
    parkingcarout.cpp \
    parkingcars.cpp \
    paymentmanagewidget.cpp \
    staffmanagewidget.cpp \
    database2.cpp \
    ownerwindow.cpp \
    pronouncementwidget.cpp \
    proaddwidget.cpp \
    database1.cpp \
    ownerrepairevalwidget.cpp

HEADERS += \
        mainwindow.h \
    staffmainwindow.h \
    repairservicewidget.h \
    devicemonitorwidget.h \
    parkingmanagerwidget.h \
    leaveapplicationwidget.h \
    activitymanagerwidget.h \
    raddwidget.h \
    showcustominputdialog.h \
    daddwidget.h \
    paddwidget.h \
    aaddwidget.h \
    approveparkingspotdialog.h \
    doubleeditdialog.h \
    processrequest.h \
    laddwidget.h \
    deviceupdatedialog.h \
    loginpage.h \
    database.h \
    accesswidget.h \
    adminwindow.h \
    attendancewidget.h \
    carmanagewidget.h \
    leaveapprovewidget.h \
    parkingcarin.h \
    parkingcarout.h \
    parkingcars.h \
    paymentmanagewidget.h \
    staffmanagewidget.h \
    database2.h \
    ownerwindow.h \
    pronouncementwidget.h \
    proaddwidget.h \
    database1.h \
    ownerrepairevalwidget.h

FORMS += \
        mainwindow.ui \
    staffmainwindow.ui \
    repairservicewidget.ui \
    parkingmanagerwidget.ui \
    leaveapplicationwidget.ui \
    activitymanagerwidget.ui \
    raddwidget.ui \
    devicemonitorwidget.ui \
    daddwidget.ui \
    paddwidget.ui \
    aaddwidget.ui \
    laddwidget.ui \
    loginpage.ui \
    accesswidget.ui \
    adminwindow.ui \
    attendancewidget.ui \
    carmanagewidget.ui \
    leaveapprovewidget.ui \
    parkingcarin.ui \
    parkingcarout.ui \
    parkingcars.ui \
    paymentmanagewidget.ui \
    staffmanagewidget.ui \
    ownerwindow.ui \
    pronouncementwidget.ui \
    proaddwidget.ui \
    ownerrepairevalwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourse.qrc

DISTFILES +=
