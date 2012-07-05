#-------------------------------------------------
#
# Project created by QtCreator 2012-06-07T12:42:26
#
#-------------------------------------------------


#-L$$PWD/../../../../../../../../usr/local/lib/ -lmorph4cpp16

QT       += core gui

TARGET = Transito
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    util.cpp \
    imageprocessor.cpp

HEADERS  += mainwindow.h \
    util.h \
    imageprocessor.h

FORMS    += mainwindow.ui


macx: LIBS += -L$$PWD/opencv241/lib/ -lopencv_core\
              -L$$PWD/opencv241/lib/ -lopencv_contrib\
              -L$$PWD/opencv241/lib/ -lopencv_imgproc\
              -L$$PWD/opencv241/lib/ -lopencv_ml\
              -L$$PWD/opencv241/lib/ -lopencv_objdetect\
              -L$$PWD/opencv241/lib/ -lopencv_video\
              -L$$PWD/opencv241/lib/ -lopencv_photo\
              -L$$PWD/opencv241/lib/ -lopencv_ts\
              -L$$PWD/opencv241/lib/ -lopencv_highgui\

INCLUDEPATH += $$PWD/opencv241/include/opencv2
DEPENDPATH += $$PWD/opencv241/include/opencv2

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/include

INCLUDEPATH += $$PWD/opencv241/include
DEPENDPATH += $$PWD/opencv241/include

OTHER_FILES += \
    lembretes.txt \
    temp.txt


