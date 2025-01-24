#include "mastershowinfoform.h"
#include "ui_mastershowinfoform.h"

MasterShowInfoForm::MasterShowInfoForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterShowInfoForm)
{
    ui->setupUi(this);
}

MasterShowInfoForm::~MasterShowInfoForm()
{
    delete ui;
}
