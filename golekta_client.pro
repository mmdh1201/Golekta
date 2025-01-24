QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcourseform.cpp \
    addmasterform.cpp \
    addstudentform.cpp \
    adminmainwindow.cpp \
    coursemanagementform.cpp \
    examform.cpp \
    global.cpp \
    homeworkform.cpp \
    main.cpp \
    mainwindow.cpp \
    professorcourselistform.cpp \
    professorgrade.cpp \
    professorinfoform.cpp \
    professormainwindow.cpp \
    professorparticipants.cpp \
    showinfoform.cpp \
    studentcourseinfo.cpp \
    studentexam.cpp \
    studenthomework.cpp \
    studentmainwindow.cpp \
    studentregisterform.cpp \
    studentshowinfoform.cpp

HEADERS += \
    addcourseform.h \
    addmasterform.h \
    addstudentform.h \
    adminmainwindow.h \
    coursemanagementform.h \
    examform.h \
    global.h \
    homeworkform.h \
    mainwindow.h \
    professorcourselistform.h \
    professorgrade.h \
    professorinfoform.h \
    professormainwindow.h \
    professorparticipants.h \
    showinfoform.h \
    studentcourseinfo.h \
    studentexam.h \
    studenthomework.h \
    studentmainwindow.h \
    studentregisterform.h \
    studentshowinfoform.h

FORMS += \
    addcourseform.ui \
    addmasterform.ui \
    addstudentform.ui \
    adminmainwindow.ui \
    coursemanagementform.ui \
    examform.ui \
    homeworkform.ui \
    mainwindow.ui \
    professorcourselistform.ui \
    professorgrade.ui \
    professorinfoform.ui \
    professormainwindow.ui \
    professorparticipants.ui \
    showinfoform.ui \
    studentcourseinfo.ui \
    studentexam.ui \
    studenthomework.ui \
    studentmainwindow.ui \
    studentregisterform.ui \
    studentshowinfoform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
