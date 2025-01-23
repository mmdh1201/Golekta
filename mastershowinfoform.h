#ifndef MASTERSHOWINFOFORM_H
#define MASTERSHOWINFOFORM_H

#include <QWidget>

namespace Ui {
class MasterShowInfoForm;
}

class MasterShowInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit MasterShowInfoForm(QWidget *parent = nullptr);
    ~MasterShowInfoForm();

private:
    Ui::MasterShowInfoForm *ui;
};

#endif // MASTERSHOWINFOFORM_H
