#include "studentstatus.h"
#include "ui_studentstatus.h"
#include "global.h"

studentstatus::studentstatus(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::studentstatus)
{

    loadinfo();

}

studentstatus::~studentstatus()
{
    delete ui;
}
void studentstatus::loadinfo(){
    ui->setupUi(this);
    ui->namelabel->setText(::stdname);
    ui->passedunits->setText(::stdcompletedUnits);
    ui->usernameLabel->setText(::stdusername);
    ui->faculty->setText(::stdfaculty);
    ui->email->setText(::stdemail);
}
