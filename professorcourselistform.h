#ifndef PROFESSORCOURSELISTFORM_H
#define PROFESSORCOURSELISTFORM_H

#include <QDialog>
#include <QListWidget>
#include "coursemanagementform.h"


namespace Ui {
class ProfessorCourseListForm;
}

class ProfessorCourseListForm : public QDialog
{
    Q_OBJECT

public:
    explicit ProfessorCourseListForm(QWidget *parent = nullptr);
    ~ProfessorCourseListForm();
    void setprofname();

private slots:
    void onCourseSelected(QListWidgetItem *item);

private:
    Ui::ProfessorCourseListForm *ui;
    QString professorName;
    void showEvent(QShowEvent *event) override{
        QWidget::showEvent(event);
        this->setprofname();
    }
    void loadCourses();
    CourseManagementForm* CMForm = nullptr;
};

#endif
