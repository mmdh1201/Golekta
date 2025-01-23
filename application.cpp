#include "application.h"

application* application::myInstance = (application*)(QApplication::instance());
application::application(int argc, char* argv[])
    : QApplication(argc,argv)
{
    this->loginPage = new MainWindow();
    this->stdpannel = new studentpannel();
    this->profmain = new ProfessorMainWindow;
    this->adminpannel = new AdminMainWindow;
    this->addcourseform = new AddCourseForm;
    this->addmasterform = new AddMasterForm;
    this->addstdform = new AddStudentForm;
    this->profcrsform = new ProfessorCourseListForm;
    this->profinfo = new ProfessorInfoForm;
    this->stdstat = new studentstatus;


}

void application::init()
{
    application::myInstance= (application*)(QApplication::instance());
    this->getlogin()->show();
}


