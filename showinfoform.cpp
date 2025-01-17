#include "showinfoform.h"
#include "ui_showinfoform.h"

ShowInfoForm::ShowInfoForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowInfoForm)
{
    ui->setupUi(this);
    ui->usernameValue->setText("admin");
    ui->passwordLineEdit->setText("1234");
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    ui->roleValue->setText("admin");

    connect(ui->togglePasswordButton, &QPushButton::clicked, this, &ShowInfoForm::togglePasswordVisibility);
}

ShowInfoForm::~ShowInfoForm()
{
    delete ui;
}

void ShowInfoForm::togglePasswordVisibility()
{
    if (ui->passwordLineEdit->echoMode() == QLineEdit::Password) {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->togglePasswordButton->setText("Hide Password");
    } else {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        ui->togglePasswordButton->setText("Show Password");
    }
}
