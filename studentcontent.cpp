#include "studentcontent.h"
#include "ui_studentcontent.h"

StudentContent::StudentContent(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentContent)
{
    ui->setupUi(this);
}

StudentContent::~StudentContent()
{
    delete ui;
}
