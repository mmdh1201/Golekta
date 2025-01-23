#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"

AdminMainWindow::AdminMainWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdminMainWindow)
{
    ui->setupUi(this);
    connect(ui->showInfoButton, &QPushButton::clicked, this, &AdminMainWindow::on_showInfoButton_clicked);
    connect(ui->addStudentButton, &QPushButton::clicked, this, &AdminMainWindow::on_addStudentButton_clicked);
    connect(ui->addMasterButton, &QPushButton::clicked, this, &AdminMainWindow::on_addMasterButton_clicked);
    connect(ui->addCourseButton, &QPushButton::clicked, this, &AdminMainWindow::on_addCourseButton_clicked);
}

AdminMainWindow::~AdminMainWindow()
{
    delete ui;
}

void AdminMainWindow::on_showInfoButton_clicked()
{
    if (!SIform) {
        SIform = new ShowInfoForm(this);
        connect(SIform, &QWidget::destroyed, this, [&]() {
            SIform = nullptr;
        });
    }
    SIform->show();
}


void AdminMainWindow::on_addStudentButton_clicked()
{
    if (!ASform) {
        ASform = new AddStudentForm(this);
        connect(ASform, &QWidget::destroyed, this, [&]() {
            ASform = nullptr;
        });
    }
    ASform->show();
}

void AdminMainWindow::on_addMasterButton_clicked()
{
    if (!AMform) {
        AMform = new AddMasterForm(this);
        connect(AMform, &QWidget::destroyed, this, [&]() {
            AMform = nullptr;
        });
    }
    AMform->show();
}

void AdminMainWindow::on_addCourseButton_clicked()
{
    if (!ACform) {
        ACform = new AddCourseForm(this);
        connect(ACform, &QWidget::destroyed, this, [&]() {
            ACform = nullptr;
        });
    }
    ACform->show();
}
