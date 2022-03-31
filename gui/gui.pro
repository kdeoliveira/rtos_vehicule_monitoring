QT += quick
CONFIG += qmltypes



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    inputdata.cpp \
    main.cpp \
    reader.cpp

RESOURCES += qml.qrc


QML_IMPORT_NAME = qnx.rtos
QML_IMPORT_MAJOR_VERSION = 1

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



qnx: {

    DEFINES += _QNX_x86_64
    qnx:!macx: LIBS += -L$$PWD/../build_qnx/lib/rtos_common/ -lrtos_common
    DEPENDPATH += $$PWD/../build_qnx/lib/rtos_common
}
unix:!qnx: {
    TARGET = gui
    unix:!macx: LIBS += -L$$PWD/../build/lib/rtos_common/ -lrtos_common -lrt
    DEPENDPATH += $$PWD/../build/lib/rtos_common
}


INCLUDEPATH += $$PWD/../lib/rtos_common/include
INCLUDEPATH += $$PWD/../lib/rtos_ipc/include
INCLUDEPATH += $$PWD/../lib/rtos_data/include



HEADERS += \
    consumer.h \
    inputdata.h \
    reader.h \
    ../src/common.hpp \
    scheduler.h

