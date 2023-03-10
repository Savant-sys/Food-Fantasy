QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Database.cpp \
    listofrestaurants.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    menuwidget.cpp \
    restaurant.cpp \
    restaurantwidget.cpp \
    trip.cpp

HEADERS += \
    Database.h \
    listofrestaurants.h \
    login.h \
    mainwindow.h \
    menuwidget.h \
    restaurant.h \
    restaurantwidget.h \
    trip.h

FORMS += \
    listofrestaurants.ui \
    login.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
