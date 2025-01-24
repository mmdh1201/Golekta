#include "addstudentform.h"
#include "ui_addstudentform.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>

AddStudentForm::AddStudentForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddStudentForm)
{
    ui->setupUi(this);
}

AddStudentForm::~AddStudentForm()
{
    delete ui;
}



void AddStudentForm::on_submitButton_clicked()
{
    QTcpSocket *socket;
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString fullName = ui->fullNameLineEdit->text();
    QString faculty = ui->facultyLineEdit->text();
    QString completedUnits = ui->completedUnitsLineEdit->text();
    QString email = ui->emailLineEdit->text();

    if (username.isEmpty() || password.isEmpty() || fullName.isEmpty() || faculty.isEmpty() || completedUnits.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled.");
        return;
    }


    QRegularExpression fullNameRegex("^\\S+ \\S+$");
    if (!fullNameRegex.match(fullName).hasMatch()) {
        QMessageBox::warning(this, "Name Error", "Please enter both first and last name separated by a space.");
        return;
    }

    QString role = "student";

    QJsonObject newUser;
    newUser["type"] = "add_user";
    newUser["username"] = username;
    newUser["password"] = password;
    newUser["first_name"] = fullName.split(" ")[0];
    newUser["last_name"] = fullName.split(" ")[1];
    newUser["faculty"] = faculty;
    newUser["email"] = email;
    newUser["completed_units"] = completedUnits.toInt();
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
            QString message = jsonResponse["message"].toString();

            if (messageType == "error") {
                QMessageBox::critical(this, "Error", message);
            } else if (messageType == "success") {
                QMessageBox::information(this, "Success", message);
            }
        }

        socket->disconnectFromHost();
    } else {
        qDebug() << "Error: " << socket->errorString();
        QMessageBox::critical(this, "Connection Error", "Could not connect to server.");
    }

    socket->close();
}
