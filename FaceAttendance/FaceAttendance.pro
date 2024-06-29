
#tcp通信增加network
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#添加opencv，seetaface头文件
INCLUDEPATH += E:\code\study_data_project\Arm+Qt+opencv\opencv452\include
INCLUDEPATH += E:\code\study_data_project\Arm+Qt+opencv\opencv452\include\opencv2
INCLUDEPATH += E:\code\study_data_project\Arm+Qt+opencv\SeetaFace\include
INCLUDEPATH += E:\code\study_data_project\Arm+Qt+opencv\SeetaFace\include\seeta
#添加opencv，seetace的库

LIBS+=E:\code\study_data_project\Arm+Qt+opencv\opencv452\x64\mingw\lib\libopencv*
LIBS+=E:\code\study_data_project\Arm+Qt+opencv\SeetaFace\lib\libSeeta*



SOURCES += \
    main.cpp \
    faceattendence.cpp

HEADERS += \
    faceattendence.h

FORMS += \
    faceattendence.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
