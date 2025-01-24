#include "professorgrade.h"
#include "ui_professorgrade.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

ProfessorGrade::ProfessorGrade(const QString &courseKey, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfessorGrade),
    courseKey(courseKey)
{
    ui->setupUi(this);

    loadParticipants();

    connect(ui->submitButton, &QPushButton::clicked, this, &ProfessorGrade::onSubmitButtonClicked);
}

ProfessorGrade::~ProfessorGrade()
{
    delete ui;
}

void ProfessorGrade::loadParticipants()
{
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
        return;
    }

    QJsonObject request;
    request["type"] = "get_participants";
    request["course_key"] = courseKey;

    QJsonDocument requestDoc(request);
    socket.write(requestDoc.toJson());
    socket.waitForBytesWritten();

    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "خطا", "پاسخی از سرور دریافت نشد.");
        return;
    }

    QByteArray response = socket.readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(response);
    QJsonObject responseObject = responseDoc.object();

    if (responseObject["type"].toString() == "participants_list") {
        QJsonArray participants = responseObject["participants"].toArray();

        for (const QJsonValue &value : participants) {
            QJsonObject participant = value.toObject();
            ui->participantsComboBox->addItem(participant["username"].toString());
        }
    } else {
        QString errorMessage = responseObject["message"].toString();
        QMessageBox::critical(this, "خطا", errorMessage);
    }

    socket.disconnectFromHost();
}

void ProfessorGrade::onSubmitButtonClicked()
{
    QString studentUsername = ui->participantsComboBox->currentText();
    int grade = ui->gradeLineEdit->text().toInt();

    if (studentUsername.isEmpty() || grade < 0 || grade > 100) {
        QMessageBox::warning(this, "خطا", "لطفاً اطلاعات را به درستی وارد کنید.");
        return;
    }

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
        return;
    }

    QJsonObject request;
    request["type"] = "set_student_grade";
    request["course_key"] = courseKey;
    request["student_username"] = studentUsername;
    request["grade"] = grade;

    QJsonDocument requestDoc(request);
    socket.write(requestDoc.toJson());
    socket.waitForBytesWritten();

    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "خطا", "پاسخی از سرور دریافت نشد.");
        return;
    }

    QByteArray response = socket.readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(response);
    QJsonObject responseObject = responseDoc.object();

    if (responseObject["type"].toString() == "success") {
        QMessageBox::information(this, "موفقیت", "نمره با موفقیت ثبت شد.");
    } else {
        QString errorMessage = responseObject["message"].toString();
        QMessageBox::critical(this, "خطا", errorMessage);
    }

    socket.disconnectFromHost();
}
