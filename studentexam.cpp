#include "studentexam.h"
#include "ui_studentexam.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

StudentExam::StudentExam(const QString &courseKey, const QString &studentUsername, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentExam),
    courseKey(courseKey),
    studentUsername(studentUsername)
{
    ui->setupUi(this);

    loadExamQuestion();

    connect(ui->submitButton, &QPushButton::clicked, this, &StudentExam::onSubmitButtonClicked);
}

StudentExam::~StudentExam()
{
    delete ui;
}

void StudentExam::loadExamQuestion()
{
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
    }

    QJsonObject request;
    request["type"] = "get_exam_question";
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

    if (responseObject["type"].toString() == "exam_question") {
        QString question = responseObject["exam_question"].toString();
        ui->questionTextEdit->setText(question);
    } else {
        QMessageBox::critical(this, "خطا", "سوال آزمون یافت نشد.");
        ui->submitButton->hide();
        return;
    }
    ui->submitButton->show();

    socket.disconnectFromHost();
}

void StudentExam::onSubmitButtonClicked()
{
    QString answer = ui->answerTextEdit->toPlainText();
    if (answer.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً پاسخ خود را وارد کنید.");
        return;
    }

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
        return;
    }

    QJsonObject request;
    request["type"] = "submit_exam_answer";
    request["course_key"] = courseKey;
    request["student_username"] = studentUsername;
    request["answer"] = answer;

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
        QMessageBox::information(this, "موفقیت", "پاسخ شما با موفقیت ثبت شد.");
        close();
    } else {
        QMessageBox::critical(this, "خطا", responseObject["message"].toString());
    }

    socket.disconnectFromHost();
}


