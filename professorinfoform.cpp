#include "professorinfoform.h"
#include "ui_professorinfoform.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include "global.h"

ProfessorInfoForm::ProfessorInfoForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfessorInfoForm)
{
    ui->setupUi(this);
    loadProfessorInfo();
}

ProfessorInfoForm::~ProfessorInfoForm()
{
    delete ui;
}

void ProfessorInfoForm::loadProfessorInfo()
{
    // QString username = ::professorUserName;

    // QTcpSocket socket;
    // socket.connectToHost("127.0.0.1", 12345);

    // if (socket.waitForConnected(3000)) {
    //     QJsonObject request;
    //     request["type"] = "get_professor_info";
    //     request["username"] = username;
    //     QJsonDocument doc(request);
    //     socket.write(doc.toJson());
    //     socket.waitForBytesWritten();

    //     if (socket.waitForReadyRead(3000)) {
    //         QByteArray response = socket.readAll();
    //         QJsonDocument responseDoc = QJsonDocument::fromJson(response);
    //         QJsonObject professorInfo = responseDoc.object();

    //         if (professorInfo["type"].toString() == "professor_info") {
    //             ui->nameLabel->setText("نام: " + professorInfo["name"].toString());
    //             ui->facultyLabel->setText("دانشکده: " + professorInfo["faculty"].toString());
    //             ui->emailLabel->setText("ایمیل: " + professorInfo["email"].toString());
    //             ui->officePhoneLabel->setText("تلفن دفتر: " + professorInfo["office_phone"].toString());
    //             ui->officeNumberLabel->setText("شماره دفتر: " + professorInfo["office_number"].toString());
    //             professorName = professorInfo["name"].toString();
    //         } else {
    //             QMessageBox::critical(this, "Error", "Professor not found.");
    //         }
    //     }
    // } else {
    //     qDebug() << "Error: " << socket.errorString();
    //     QMessageBox::critical(this, "Connection Error", "Could not connect to server.");
    // }

    // socket.disconnectFromHost();

    ui->nameLabel->setText("نام: " + ::professorName);
    ui->facultyLabel->setText("دانشکده: " + ::professorFaculty);
    ui->emailLabel->setText("ایمیل: " + professorEmail);
    ui->officePhoneLabel->setText("تلفن دفتر: " + professorOfficePhoneNumber);
    ui->officeNumberLabel->setText("شماره دفتر: " + professorOfficeNumber);
}
