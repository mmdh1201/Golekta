#include "professorparticipants.h"
#include "ui_professorparticipants.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

ProfessorParticipants::ProfessorParticipants(const QString &courseKey, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfessorParticipants),
    courseKey(courseKey)
{
    ui->setupUi(this);
    loadParticipants();
}

ProfessorParticipants::~ProfessorParticipants()
{
    delete ui;
}

void ProfessorParticipants::loadParticipants()
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
            QString participantInfo = participant["username"].toString() +
                                      " (نمره: " + QString::number(participant["grade"].toInt()) + ")";
            ui->participantsListWidget->addItem(participantInfo);
        }
    } else {
        QString errorMessage = responseObject["message"].toString();
        QMessageBox::critical(this, "خطا", errorMessage);
    }

    socket.disconnectFromHost();
}
