QT += core gui charts widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CsvReader.cpp \
        DataImportDialog.cpp \
        main.cpp \
        SignalInterface.cpp \
        qcustomchartview.cpp

HEADERS += \
        CsvReader.h \
        DataImportDialog.h \
        SignalInterface.h \
        qcustomchartview.h

FORMS += \
        DataImportDialog.ui \
        SignalInterface.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_CXXFLAGS += -Wa,-mbig-obj

QMAKE_CXXFLAGS += -Wno-deprecated-declarations