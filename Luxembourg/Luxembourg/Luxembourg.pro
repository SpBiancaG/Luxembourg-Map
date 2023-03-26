QT       += core gui
\xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Arc.cpp \
    Graf.cpp \
    Node.cpp \
    main.cpp \
    Luxembourg.cpp

HEADERS += \
    Arc.h \
    Graf.h \
    Luxembourg.h \
    Node.h

FORMS += \
    Luxembourg.ui


QT += xml


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../../Downloads/Harta_Luxemburg.xml \
    Harta_Luxemburg.xml \
    d.txt
