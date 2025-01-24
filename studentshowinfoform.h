#ifndef STUDENTSHOWINFOFORM_H
#define STUDENTSHOWINFOFORM_H

#include <QDialog>

namespace Ui {
class StudentShowInfoForm;
}

class StudentShowInfoForm : public QDialog
{
    Q_OBJECT

public:
    explicit StudentShowInfoForm(QWidget *parent = nullptr);
    ~StudentShowInfoForm();
    void loadStudentInfo();

private:
    Ui::StudentShowInfoForm *ui;
};

#endif // STUDENTSHOWINFOFORM_H
