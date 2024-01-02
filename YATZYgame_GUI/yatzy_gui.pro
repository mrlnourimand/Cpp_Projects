QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    functions.cpp \
    gameengine.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    functions.hh \
    gameengine.hh \
    mainwindow.hh

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Resources/1.png \
    Resources/2.png \
    Resources/3.png \
    Resources/4.png \
    Resources/5.png \
    Resources/6.png \
    Resources/empty.png

RESOURCES += \
    diceresources.qrc
