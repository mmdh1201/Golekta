#include "examform.h"
#include "ui_examform.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

ExamForm::ExamForm(const QString &courseName, const QString &professorName, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExamForm)
    , courseName(courseName)
    , professorName(professorName)
{
    ui->setupUi(this);

    connect(ui->submitButton, &QPushButton::clicked, this, &ExamForm::onSubmitButtonClicked);
}

ExamForm::~ExamForm()
{
    delete ui;
}

void ExamForm::onSubmitButtonClicked()
{
    QString examQuestion = ui->examQuestionTextEdit->toPlainText().trimmed();
    QString startingTime = ui->startingLineEdit->text();
    QString endingTime = ui->endingLineEdit->text();

    if (examQuestion.isEmpty() || startingTime.isEmpty() || endingTime.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please fill in all fields.");
        return;
    }

    sendExamDetailsToServer(examQuestion, startingTime, endingTime);
}

void ExamForm::sendExamDetailsToServer(const QString &examQuestion, const QString &startingTime, const QString &endingTime)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected to server";

        QJsonObject request;
        request["type"] = "submit_exam";
        request["course_name"] = courseName;
        request["professor_name"] = professorName;
        request["exam_question"] = examQuestion;
        request["starting_time"] = startingTime;
        request["ending_time"] = endingTime;

        QJsonDocument requestDoc(request);
        QByteArray requestData = requestDoc.toJson();

        socket->write(requestData);
        socket->flush();

        if (socket->waitForReadyRead(3000)) {
            QByteArray response = socket->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject responseObj = responseDoc.object();

            if (responseObj["type"].toString() == "success") {
                QMessageBox::information(this, "Success", "Exam details submitted successfully.");
                this->close();
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
