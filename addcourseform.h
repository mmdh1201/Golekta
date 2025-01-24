#ifndef ADDCOURSEFORM_H
#define ADDCOURSEFORM_H

#include <QDialog>

namespace Ui {
class AddCourseForm;
}

class AddCourseForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddCourseForm(QWidget *parent = nullptr);
    ~AddCourseForm();

private:
    Ui::AddCourseForm *ui;
private slots:
    void on_submitButton_clicked();
    void loadInstructors();
};

#endif
