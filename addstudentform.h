#ifndef ADDSTUDENTFORM_H
#define ADDSTUDENTFORM_H

#include <QDialog>

namespace Ui {
class AddStudentForm;
}

class AddStudentForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudentForm(QWidget *parent = nullptr);
    ~AddStudentForm();

private:
    Ui::AddStudentForm *ui;
private slots:
    void on_submitButton_clicked();
};

#endif // ADDSTUDENTFORM_H
