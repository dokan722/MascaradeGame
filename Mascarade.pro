TEMPLATE = app
TARGET = Maskarada

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    Player.cpp \
    gamewindow.cpp \
    main.cpp \
    startingwindow.cpp

HEADERS += \
    gamewindow.h \
    startingwindow.h
