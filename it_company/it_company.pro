TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    employee.cpp \
    date.cpp \
    project.cpp \
    company.cpp \
    cli.cpp \
    utils.cpp

HEADERS += \
    employee.hh \
    date.hh \
    project.hh \
    company.hh \
    cli.hh \
    utils.hh
