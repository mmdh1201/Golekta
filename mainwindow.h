#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "showinfoform.h"
#include "addmasterform.h"
#include "addstudentform.h"
#include "registercoursesform.h"
#include "addcourseform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_showInfoButton_clicked();
    void on_addStudentButton_clicked();
    void on_addMasterButton_clicked();
    void on_addCourseButton_clicked();

private:
    ShowInfoForm *SIform = nullptr;
    AddMasterForm *AMform = nullptr;
    AddStudentForm *ASform = nullptr;
    RegisterCoursesForm *RCform = nullptr;
    AddCourseForm *ACform = nullptr;
    Ui::MainWindow *ui;
};

#endif
