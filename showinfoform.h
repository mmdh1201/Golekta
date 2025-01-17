#ifndef SHOWINFOFORM_H
#define SHOWINFOFORM_H

#include <QDialog>

namespace Ui {
class ShowInfoForm;
}

class ShowInfoForm : public QDialog
{
    Q_OBJECT

public:
    explicit ShowInfoForm(QWidget *parent = nullptr);
    ~ShowInfoForm();

private:
    Ui::ShowInfoForm *ui;

private slots:
    void togglePasswordVisibility();
};

#endif
