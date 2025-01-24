#ifndef ADMINMAINWINDOW_H
#define ADMINMAINWINDOW_H

#include <QDialog>
#include "showinfoform.h"
#include "addmasterform.h"
#include "addstudentform.h"
#include "registercoursesform.h"
#include "addcourseform.h"

namespace Ui {
class AdminMainWindow;
}

class AdminMainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdminMainWindow(QWidget *parent = nullptr);
    ~AdminMainWindow();

private slots:
    void on_showInfoButton_clicked();
    void on_addStudentButton_clicked();
    void on_addMasterButton_clicked();
    void on_addCourseButton_clicked();

private:
    Ui::AdminMainWindow *ui;
    ShowInfoForm *SIform = nullptr;
    AddMasterForm *AMform = nullptr;
    AddStudentForm *ASform = nullptr;
    RegisterCoursesForm *RCform = nullptr;
    AddCourseForm *ACform = nullptr;
};

#endif // ADMINMAINWINDOW_H
