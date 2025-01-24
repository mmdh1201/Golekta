#ifndef COURSEMANAGEMENTFORM_H
#define COURSEMANAGEMENTFORM_H

#include <QDialog>
#include "homeworkform.h"
#include "examform.h"
#include "professorparticipants.h"
#include "professorgrade.h"

namespace Ui {
class CourseManagementForm;
}

class CourseManagementForm : public QDialog
{
    Q_OBJECT

public:
    explicit CourseManagementForm(const QString &courseName, QWidget *parent = nullptr);
    ~CourseManagementForm();

private slots:
    void onParticipantsButtonClicked();
    void onContentButtonClicked();
    void onExamButtonClicked();
    void onAssignmentButtonClicked();
    void onGradesButtonClicked();
    void uploadFile(const QString &fileName, const QString &requestType);

private:
    Ui::CourseManagementForm *ui;
    QString courseName;
    HomeworkForm* HWForm = nullptr;
    ExamForm* EXForm = nullptr;
    ProfessorParticipants* PARForm = nullptr;
    ProfessorGrade* GRForm = nullptr;
};

#endif
