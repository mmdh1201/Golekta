#include "studenthomework.h"
#include "ui_studenthomework.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

StudentHomeWork::StudentHomeWork(const QString &courseKey, const QString &studentUsername, QWidget *parent)
    : QDialog(parent),
    courseKey(courseKey),
    studentUsername(studentUsername)
    , ui(new Ui::StudentHomeWork)
{
    ui->setupUi(this);
    loadHomeWorkQuestion();
    connect(ui->submitButton, &QPushButton::clicked, this, &StudentHomeWork::onSubmitButtonClicked);
}

StudentHomeWork::~StudentHomeWork()
{
    delete ui;
}

void StudentHomeWork::loadHomeWorkQuestion()
{
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
    }

    QJsonObject request;
    request["type"] = "get_homework_question";
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

    if (responseObject["type"].toString() == "homework_text") {
        qDebug()<<"1";
        QString question = responseObject["homework_text"].toString();
        ui->questionTextEdit->setText(question);
    } else {
        QMessageBox::critical(this, "خطا", "سوال تکلیف یافت نشد.");
        ui->submitButton->hide();
        return;
    }
    ui->submitButton->show();

    socket.disconnectFromHost();
}

void StudentHomeWork::onSubmitButtonClicked()
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
    request["type"] = "submit_homework_answer";
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

