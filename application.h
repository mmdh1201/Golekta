#ifndef APPLICATION_H
#define APPLICATION_H
//with application you can manage all widgets in your app
#include <QApplication>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include "mainwindow.h"
#include "studentpannel.h"
#include "studentstatus.h"
#include "coursemanagementform.h"
#include "addcourseform.h"
#include "addmasterform.h"
#include "addstudentform.h"
#include "adminmainwindow.h"
#include "examform.h"
#include "homeworkform.h"
#include "professormainwindow.h"
#include "professorcourselistform.h"
#include "professorinfoform.h"




using namespace std;

class MainWindow;
class studentpannel;
class CourseManagementForm;
class AddCourseForm;
class AddStudentForm;
class AddMasterForm;
class AdminMainWindow;
class examsform;
class HomeworkForm;
class ProfessorMainWindow;
class ProfessorCourseListForm;
class ProfessorInfoForm;
class studentstatus;




class application : public QApplication
{
    Q_OBJECT

public:

    static application *myInstance;
    static application* instace(){return application::myInstance;}
    application(int argc, char *argv[]);
    MainWindow* getlogin(){return loginPage;}
    studentpannel* getpannel(){return stdpannel;}
    CourseManagementForm* getcrsform(){return master;}
    AddCourseForm* getaddcourse(){return addcourseform;}
    AddMasterForm* getaddmaster(){return addmasterform;}
    AddStudentForm* getaddstd(){return addstdform;}
    AdminMainWindow* getadmin(){return adminpannel;}
    examsform* getexamsform(){return examsform;}
    HomeworkForm* getHWform(){return hwform;}
    ProfessorMainWindow* getmainprof(){return profmain;}
    ProfessorCourseListForm* getprofcrslist(){return profcrsform;}
    ProfessorInfoForm* getprofinfo(){return profinfo;}
    studentstatus *getstdstat(){return stdstat;}
    void init();
    QString stdid;
    QString profName;

private:
    MainWindow * loginPage;
    studentpannel *stdpannel;
    studentstatus *stdstat;
    CourseManagementForm *master;
    AddCourseForm *addcourseform;
    AddMasterForm *addmasterform;
    AddStudentForm *addstdform;
    AdminMainWindow *adminpannel;
    examsform *examsform;
    HomeworkForm *hwform;
    ProfessorMainWindow *profmain;
    ProfessorCourseListForm *profcrsform;
    ProfessorInfoForm *profinfo;






};
static inline application *app(){
    return application::instace();
}

#endif // APPLICATION_H
