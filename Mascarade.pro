TEMPLATE = app
TARGET = Maskarada

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    game.cpp \
    gamewindow.cpp \
    main.cpp \
    role.cpp \
    startingwindow.cpp

HEADERS += \
    game.h \
    gamewindow.h \
    role.h \
    startingwindow.h
