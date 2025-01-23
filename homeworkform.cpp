#include "homeworkform.h"
#include "ui_homeworkform.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

HomeworkForm::HomeworkForm(const QString &courseName, const QString &professorName, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HomeworkForm)
    , courseName(courseName)
    , professorName(professorName)
{
    ui->setupUi(this);


    connect(ui->submitButton, &QPushButton::clicked, this, &HomeworkForm::onSubmitButtonClicked);
}

HomeworkForm::~HomeworkForm()
{
    delete ui;
}

void HomeworkForm::onSubmitButtonClicked()
{
    QString homeworkText = ui->homeworkTextEdit->toPlainText().trimmed();

    if (homeworkText.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter some text for the homework.");
        return;
    }

    sendHomeworkToServer(homeworkText);
}

void HomeworkForm::sendHomeworkToServer(const QString &homeworkText)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected to server";

        QJsonObject request;
        request["type"] = "submit_homework";
        request["course_name"] = courseName;
        request["professor_name"] = professorName;
        request["homework_text"] = homeworkText;

        QJsonDocument requestDoc(request);
        QByteArray requestData = requestDoc.toJson();

        socket->write(requestData);
        socket->flush();

        if (socket->waitForReadyRead(3000)) {
            QByteArray response = socket->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject responseObj = responseDoc.object();

            if (responseObj["type"].toString() == "success") {
                QMessageBox::information(this, "Success", "Homework submitted successfully.");
                this->accept();
            } else {
                QMessageBox::critical(this, "Error", responseObj["message"].toString());
            }
        }
    } else {
        qDebug() << "Error: " << socket->errorString();
        QMessageBox::critical(this, "Connection Error", "Could not connect to server.");
    }

    socket->close();
}
