#include "studentshowinfoform.h"
#include "ui_studentshowinfoform.h"
#include "global.h"

StudentShowInfoForm::StudentShowInfoForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StudentShowInfoForm)
{
    ui->setupUi(this);
    loadStudentInfo();
}

StudentShowInfoForm::~StudentShowInfoForm()
{
    delete ui;
}

void StudentShowInfoForm::loadStudentInfo(){
    ui->completedUnitsLineEdit->setText(::studentPassedUnits);
    ui->facultyLineEdit->setText(::studentFaculty);
    ui->passwordLineEdit->setText(::studentPassword);
    ui->usernameLineEdit->setText(::studentUserName);
    ui->nameLineEdit->setText(::studentName);
}
