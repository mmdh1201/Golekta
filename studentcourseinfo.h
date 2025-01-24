#ifndef STUDENTCOURSEINFO_H
#define STUDENTCOURSEINFO_H

#include <QDialog>
#include "studentexam.h"
#include "studenthomework.h"

namespace Ui {
class StudentCourseInfo;
}

class StudentCourseInfo : public QDialog
{
    Q_OBJECT

public:
    explicit StudentCourseInfo(const QString &courseName, const QString &courseMaster, QWidget *parent = nullptr);
    ~StudentCourseInfo();

private slots:
    void onViewAssignmentClicked();
    void onViewExamClicked();
    void onViewContentClicked();
    void onViewGradeClicked();

private:
    Ui::StudentCourseInfo *ui;
    QString courseName;
    QString courseMaster;
    StudentExam* STDEXForm = nullptr;
    StudentHomeWork* STDHWForm = nullptr;
};

#endif // STUDENTCOURSEINFO_H
