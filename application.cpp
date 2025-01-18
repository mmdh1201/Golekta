#include "application.h"
#include "mainwindow.h"
#include <algorithm>
#include <fstream>
#include <iostream>


application* application::myInstance = (application*)(QApplication::instance());
application::application(int argc, char* argv[])
    : QApplication(argc,argv)
{
    this->loginPage = new MainWindow();
}

void application::init()
{
    application::myInstance= (application*)(QApplication::instance());
    this->getlogin()->show();
}


