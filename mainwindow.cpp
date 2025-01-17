#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showinfoform.h"
#include "addmasterform.h"
#include "addstudentform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->showInfoButton, &QPushButton::clicked, this, &MainWindow::on_showInfoButton_clicked);
    connect(ui->addStudentButton, &QPushButton::clicked, this, &MainWindow::on_addStudentButton_clicked);
    connect(ui->addMasterButton, &QPushButton::clicked, this, &MainWindow::on_addMasterButton_clicked);
    connect(ui->addCourseButton, &QPushButton::clicked, this, &MainWindow::on_addCourseButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_showInfoButton_clicked()
{
    if (!SIform) {
        SIform = new ShowInfoForm(this);
        connect(SIform, &QWidget::destroyed, this, [&]() {
            SIform = nullptr;
        });
    }
    SIform->show();
}


void MainWindow::on_addStudentButton_clicked()
{
    if (!ASform) {
        ASform = new AddStudentForm(this);
        connect(ASform, &QWidget::destroyed, this, [&]() {
            ASform = nullptr;
        });
    }
    ASform->show();
}

void MainWindow::on_addMasterButton_clicked()
{
    if (!AMform) {
        AMform = new AddMasterForm(this);
        connect(AMform, &QWidget::destroyed, this, [&]() {
            AMform = nullptr;
        });
    }
    AMform->show();
}

void MainWindow::on_addCourseButton_clicked()
{
    if (!ACform) {
        ACform = new AddCourseForm(this);
        connect(ACform, &QWidget::destroyed, this, [&]() {
            ACform = nullptr;
        });
    }
    ACform->show();
}
