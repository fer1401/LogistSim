QT       += core gui quickwidgets positioning location

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DSG = /home/jesus/Documentos/DeSiGNAR

INCLUDEPATH += $${DSG}/include ../

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Routes to header files for libraries
INCLUDEPATH += Include
INCLUDEPATH += /usr/include/


SOURCES += \
    Source/warehouseedit.cpp \
    Source/City.cpp \
    Source/CountHandler.cpp \
    Source/MapPoint.cpp \
    Source/Settings.cpp \
    Source/Stockspinboxdelegate.cpp \
    Source/Street.cpp \
    Source/Truck.cpp \
    Source/main.cpp \
    Source/mainwindow.cpp \
    Source/Product.cpp \
    Source/Order.cpp \
    Source/Inventory.cpp \
    Source/Warehouse.cpp \
    Source/Simulation.cpp \
    Source/Welcomedialog.cpp \

HEADERS += \
    Include/warehouseedit.h \
    Include/City.h \
    Include/CountHandler.h \
    Include/MapPoint.h \
    Include/Street.h \
    Include/Truck.h \
    Include/mainwindow.h \
    Include/Product.h \
    Include/Order.h \
    Include/Inventory.h \
    Include/Warehouse.h \
    Include/Simulation.h \
    Include/Welcomedialog.h \
    Include/Settings.h \
    Include/Stockspinboxdelegate.h

FORMS += \
    Forms/mainwindow.ui \
    Forms/warehouseedit.ui \
    Forms/welcomedialog.ui \
    Forms/settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Link libraries
LIBS += -lprotobuf -lbz2 -lexpat -lz -lgdal -L$${DSG}/lib -lDesignar -lboost_iostreams

RESOURCES += \
    res.qrc

DISTFILES += \
