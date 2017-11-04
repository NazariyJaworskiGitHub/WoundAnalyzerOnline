CONFIG += c++11
CONFIG += console

#QT += core
#QT += gui
#QT += printsupport
#QT += sql

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WoundAnalyzerOnline

#Wt Web ToolKit
CONFIG      += no_keywords # so Qt won't #define any non-all-caps 'keywords'
INCLUDEPATH += E:\Wt\Wt_3.3.7_boost_1.54.0_mingw_4.8.0_x32\include\
INCLUDEPATH += E:\Boost\boost_1_54_0
#Order of libraries is importatnt!
LIBS += E:\Wt\Wt_3.3.7_boost_1.54.0_mingw_4.8.0_x32\lib\libwthttp.a
LIBS += E:\Wt\Wt_3.3.7_boost_1.54.0_mingw_4.8.0_x32\lib\libwt.a
LIBS += E:\Boost\boost_1_54_0\lib\libboost_random-mgw48-mt-1_54.a
LIBS += E:\Boost\boost_1_54_0\lib\libboost_regex-mgw48-mt-1_54.a
LIBS += E:\Boost\boost_1_54_0\lib\libboost_signals-mgw48-mt-1_54.a
LIBS += E:\Boost\boost_1_54_0\lib\libboost_system-mgw48-mt-1_54.a
LIBS += E:\Boost\boost_1_54_0\lib\libboost_thread-mgw48-mt-1_54.a
LIBS += E:\Boost\boost_1_54_0\lib\libboost_filesystem-mgw48-mt-1_54.a
LIBS += E:\Boost\boost_1_54_0\lib\libboost_program_options-mgw48-mt-1_54.a
LIBS += E:\Boost\boost_1_54_0\lib\libboost_date_time-mgw48-mt-1_54.a
LIBS += -lws2_32
LIBS += -lwsock32

#Flags
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs #disable a lot of boost warnings

#OpenCV
INCLUDEPATH += e:\OpenCV_2.4.13\opencv\localBuild\include
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_core2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_highgui2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_imgproc2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_photo2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_video2413.dll.a

#For debugging
CONFIG(debug, release|debug):DEFINES += _DEBUG_MODE

SOURCES += main.cpp \
    Image/imagemanager.cpp \
    Web/UI/windowimageedit.cpp

HEADERS  += Utilities/Logger/logger.h \
    Web/webserver.h \
    configurationparameters.h \
    Web/usersession.h \
    patient.h \
    Utilities/countriescode.h \
    Web/UI/windowpatient.h \
    Image/imagemanager.h \
    Utilities/mathutilities.h \
    Image/imagemanagerqt.h \
    Image/imagemanagerwt.h \
    Web/UI/windowimageedit.h \
    Web/UI/dialogprogressbar.h \
    Web/fileupload.h \
    Web/UI/dialogimagesettings.h

DISTFILES +=
