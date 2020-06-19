QT += core gui widgets svg

TEMPLATE = app

CONFIG += c+11

SOURCES += \
    src/core/equipment_a_331.cpp \
    src/core/equipment_rsbn_np.cpp \
    src/core/equipment_simulator.cpp \
    src/core/navigational_equipment.cpp \
    src/core/navigational_equipment_manager.cpp \
    src/gui/main_window.cpp \
    src/main.cpp \
    src/tools/interface_ARINC429.cpp

FORMS += \
    src/gui/main_window.ui

HEADERS += \
    src/core/equipment_a_331.h \
    src/core/equipment_rsbn_np.h \
    src/core/equipment_simulator.h \
    src/core/navigational_equipment.h \
    src/core/navigational_equipment_manager.h \
    src/gui/main_window.h \
    src/tools/interface_ARINC429.h \
    src/tools/intrfacePCI3.h \
    src/tools/ioctlPCI3.h


unix|win32: LIBS += -L$$PWD/libs/ -lSetupAPI

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

RESOURCES += \
    resources.qrc
