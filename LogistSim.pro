QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Routes to header files for libraries
INCLUDEPATH += Include
INCLUDEPATH += /usr/include/


SOURCES += \
    Source/Arcinfo.cpp \
    Source/City.cpp \
    Source/Nodeinfo.cpp \
    Source/Truck.cpp \
    Source/main.cpp \
    Source/mainwindow.cpp

HEADERS += \
    Include/Arcinfo.h \
    Include/City.h \
    Include/Nodeinfo.h \
    Include/Truck.h \
    Include/mainwindow.h

FORMS += \
    Forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Link libraries
LIBS += -lprotobuf -lbz2 -lexpat -lz -lgdal
