CONFIG += c++11
CONFIG += console

#QT += core
#QT += gui
#QT += printsupport
QT += sql

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WoundAnalyzerOnline

#Wt Web ToolKit
CONFIG      += no_keywords # so Qt won't #define any non-all-caps 'keywords'
INCLUDEPATH += d:\_DEV\wt-3.3.8-mingw51-x64\include\
INCLUDEPATH += d:\_DEV\boost_1_65_1
#Order of libraries is importatnt!
LIBS += d:\_DEV\wt-3.3.8-mingw51-x64\lib\libwthttp.dll.a
LIBS += d:\_DEV\wt-3.3.8-mingw51-x64\lib\libwt.dll.a
LIBS += d:\_DEV\boost_1_65_1\lib\libboost_random-mgw51-mt-1_65_1.dll.a
LIBS += d:\_DEV\boost_1_65_1\lib\libboost_regex-mgw51-mt-1_65_1.dll.a
LIBS += d:\_DEV\boost_1_65_1\lib\libboost_signals-mgw51-mt-1_65_1.dll.a
LIBS += d:\_DEV\boost_1_65_1\lib\libboost_system-mgw51-mt-1_65_1.dll.a
LIBS += d:\_DEV\boost_1_65_1\lib\libboost_thread-mgw51-mt-1_65_1.dll.a
LIBS += d:\_DEV\boost_1_65_1\lib\libboost_filesystem-mgw51-mt-1_65_1.dll.a
LIBS += d:\_DEV\boost_1_65_1\lib\libboost_program_options-mgw51-mt-1_65_1.dll.a
LIBS += d:\_DEV\boost_1_65_1\lib\libboost_date_time-mgw51-mt-1_65_1.dll.a
LIBS += -lws2_32
LIBS += -lwsock32

#Flags
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs #disable a lot of boost warnings

#OpenCV
INCLUDEPATH += d:\_DEV\OpenCV-2.4.13.4-mingw51-x64\include
LIBS += d:\_DEV\OpenCV-2.4.13.4-mingw51-x64\lib\libopencv_core2413.dll.a
LIBS += d:\_DEV\OpenCV-2.4.13.4-mingw51-x64\lib\libopencv_highgui2413.dll.a
LIBS += d:\_DEV\OpenCV-2.4.13.4-mingw51-x64\lib\libopencv_imgproc2413.dll.a
LIBS += d:\_DEV\OpenCV-2.4.13.4-mingw51-x64\lib\libopencv_photo2413.dll.a
LIBS += d:\_DEV\OpenCV-2.4.13.4-mingw51-x64\lib\libopencv_video2413.dll.a

#For debugging
CONFIG(debug, release|debug):DEFINES += _DEBUG_MODE

SOURCES += main.cpp \
    Image/imagemanager.cpp \
    Web/UI/windowimageedit.cpp \
    DataBase/databasemanager.cpp \
    Web/UI/authmanager.cpp

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
    Web/UI/dialogimagesettings.h \
    DataBase/databasemanager.h \
    DataBase/databasemodel.h \
    Web/UI/windowintro.h \
    Web/UI/authmanager.h \
    Web/UI/mainwindow.h

DISTFILES +=
