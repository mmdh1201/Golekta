#include "addmasterform.h"
#include "ui_addmasterform.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

AddMasterForm::AddMasterForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddMasterForm)
{
    ui->setupUi(this);
}

AddMasterForm::~AddMasterForm()
{
    delete ui;
}



QTcpSocket *socket;

void AddMasterForm::on_submitButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString fullName = ui->fullNameLineEdit->text();
    QString faculty = ui->facultyLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString officePhone = ui->officePhoneLineEdit->text();
    QString officeNumber = ui->officeNumberLineEdit->text();
    QString role = "master";

    if (username.isEmpty() || password.isEmpty() || fullName.isEmpty() || faculty.isEmpty() ||
        email.isEmpty() || officePhone.isEmpty() || officeNumber.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled.");
        return;
    }


    QRegularExpression fullNameRegex("^\\S+ \\S+$");
    if (!fullNameRegex.match(fullName).hasMatch()) {
        QMessageBox::warning(this, "Name Error", "Please enter both first and last name separated by a space.");
        return;
    }
    QJsonObject newUser;
    newUser["type"] = "add_user";
    newUser["username"] = username;
    newUser["password"] = password;
    newUser["first_name"] = fullName.split(" ")[0];
    newUser["last_name"] = fullName.split(" ")[1];
    newUser["faculty"] = faculty;
    newUser["email"] = email;
    newUser["completed_units"] = -1;
    newUser["office_phone"] = officePhone;
    newUser["office_number"] = officeNumber;
    newUser["role"] = role;

    QJsonDocument doc(newUser);
    QByteArray jsonData = doc.toJson();

    socket = new QTcpSocket(this);

    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected to server";

        socket->write(jsonData);
        socket->flush();

        if (socket->waitForReadyRead(3000)) {
            QByteArray response = socket->readAll();

            QJsonDocument docResponse = QJsonDocument::fromJson(response);
            QJsonObject jsonResponse = docResponse.object();

            QString messageType = jsonResponse["type"].toString();
            QString responseMessage = jsonResponse["message"].toString();

            if (messageType == "error") {
                QMessageBox::critical(this, "Error", responseMessage);
            } else if (messageType == "success") {
                QMessageBox::information(this, "Success", responseMessage);
                ui->emailLineEdit->clear();
                ui->facultyLineEdit->clear();
                ui->fullNameLineEdit->clear();
                ui->officeNumberLineEdit->clear();
                ui->passwordLineEdit->clear();
                ui->usernameLineEdit->clear();
                ui->officePhoneLineEdit->clear();
            }
        }

        socket->disconnectFromHost();
    } else {
        qDebug() << "Error: " << socket->errorString();
        QMessageBox::critical(this, "Connection Error", "Could not connect to server.");
    }

    socket->close();

}

