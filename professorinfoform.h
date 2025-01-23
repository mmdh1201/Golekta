#ifndef PROFESSORINFOFORM_H
#define PROFESSORINFOFORM_H

#include <QDialog>

namespace Ui {
class ProfessorInfoForm;
}

class ProfessorInfoForm : public QDialog
{
    Q_OBJECT

public:
    explicit ProfessorInfoForm(QWidget *parent = nullptr);
    ~ProfessorInfoForm();
    void loadProfessorInfo();

private:
    Ui::ProfessorInfoForm *ui;
};

#endif
