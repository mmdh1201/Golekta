#ifndef ADDMASTERFORM_H
#define ADDMASTERFORM_H

#include <QDialog>

namespace Ui {
class AddMasterForm;
}

class AddMasterForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddMasterForm(QWidget *parent = nullptr);
    ~AddMasterForm();

private:
    Ui::AddMasterForm *ui;

private slots:
    void on_submitButton_clicked();
};

#endif
