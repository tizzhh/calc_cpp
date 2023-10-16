QT       += core gui
QT       += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    creditwindow.cpp \
    depositwindow.cpp \
    graphwindow.cpp \
    inputxwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    ../../c_files/s21_calculate.c \
    ../../c_files/s21_convert_to_postfix.c \
    ../../c_files/s21_stack.c \
    ../../c_files/s21_credit.c \
    ../../c_files/s21_deposit.c

HEADERS += \
    creditwindow.h \
    depositwindow.h \
    graphwindow.h \
    inputxwindow.h \
    mainwindow.h \
    qcustomplot.h \
    ../../c_files/s21_calc.h

FORMS += \
    creditwindow.ui \
    depositwindow.ui \
    graphwindow.ui \
    inputxwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
