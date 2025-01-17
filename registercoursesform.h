#ifndef REGISTERCOURSESFORM_H
#define REGISTERCOURSESFORM_H

#include <QDialog>

namespace Ui {
class RegisterCoursesForm;
}

class RegisterCoursesForm : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterCoursesForm(QWidget *parent = nullptr);
    ~RegisterCoursesForm();

private:
    Ui::RegisterCoursesForm *ui;
};

#endif
