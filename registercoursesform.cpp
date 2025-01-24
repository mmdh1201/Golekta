#include "registercoursesform.h"
#include "ui_registercoursesform.h"

RegisterCoursesForm::RegisterCoursesForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterCoursesForm)
{
    ui->setupUi(this);
}

RegisterCoursesForm::~RegisterCoursesForm()
{
    delete ui;
}
