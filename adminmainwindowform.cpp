#include "adminmainwindowform.h"
#include "ui_adminmainwindowform.h"

AdminMainWindowForm::AdminMainWindowForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminMainWindowForm)
{
    ui->setupUi(this);
}

AdminMainWindowForm::~AdminMainWindowForm()
{
    delete ui;
}
