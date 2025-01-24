#ifndef STUDENTMAINWINDOW_H
#define STUDENTMAINWINDOW_H

#include <QDialog>
#include <QListWidgetItem>
#include "studentshowinfoform.h"
#include "studentregisterform.h"
#include "studentcourseinfo.h"

namespace Ui {
class StudentMainWindow;
}

class StudentMainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StudentMainWindow(QWidget *parent = nullptr);
    ~StudentMainWindow();

private slots:
    void onRegisterTermClicked();
    void onViewInfoClicked();
    void onRemoveCourseClicked();
    void globalLoadStudentInfo();
    void refreshCourseList();
    void onCourseItemClicked(QListWidgetItem *item);

private:
    Ui::StudentMainWindow *ui;
    StudentShowInfoForm* STDSIForm = nullptr;
    StudentRegisterForm* STDRForm = nullptr;
    StudentCourseInfo* STDCForm = nullptr;
};

#endif
