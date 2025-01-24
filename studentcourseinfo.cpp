#include "studentcourseinfo.h"
#include "ui_studentcourseinfo.h"
#include <QMessageBox>
#include "global.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

StudentCourseInfo::StudentCourseInfo(const QString &courseName, const QString &courseMaster, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentCourseInfo),
    courseName(courseName),
    courseMaster(courseMaster)
{
    ui->setupUi(this);

    ui->courseNameLabel->setText("نام درس: " + courseName);
    ui->courseMasterLabel->setText("نام استاد: " + courseMaster);

    connect(ui->viewAssignmentButton, &QPushButton::clicked, this, &StudentCourseInfo::onViewAssignmentClicked);
    connect(ui->viewExamButton, &QPushButton::clicked, this, &StudentCourseInfo::onViewExamClicked);
    connect(ui->viewContentButton, &QPushButton::clicked, this, &StudentCourseInfo::onViewContentClicked);
    connect(ui->viewGradeButton, &QPushButton::clicked, this, &StudentCourseInfo::onViewGradeClicked);

}

StudentCourseInfo::~StudentCourseInfo()
{
    delete ui;
}

void StudentCourseInfo::onViewAssignmentClicked()
{
    if (!STDHWForm) {
        STDHWForm = new StudentHomeWork(::courseKey, ::studentUserName, this);
        STDHWForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(STDHWForm, &QWidget::destroyed, this, [&]() {
            STDHWForm = nullptr;
        });
    }
    STDHWForm->show();
}

void StudentCourseInfo::onViewExamClicked()
{
    if (!STDEXForm) {
        STDEXForm = new StudentExam(::courseKey, ::studentUserName, this);
        STDEXForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(STDEXForm, &QWidget::destroyed, this, [&]() {
            STDEXForm = nullptr;
        });
    }
    STDEXForm->show();
}

void StudentCourseInfo::onViewContentClicked()
{
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
    }

    QJsonObject request;
    request["type"] = "get_content";
    request["course_key"] = ::courseKey;

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

    if (responseObject["type"].toString() == "file_name") {
        qDebug()<<"1";
        QString fileName = responseObject["file_name"].toString();
        QMessageBox::information(this, "محتوا",fileName);
    } else {
        QMessageBox::critical(this, "خطا", "محتوا یافت نشد.");
        return;
    }
    socket.disconnectFromHost();
}

void StudentCourseInfo::onViewGradeClicked()
{
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
        return;
    }

    QJsonObject request;
    request["type"] = "get_student_grade";
    request["course_key"] = courseKey;
    request["student_username"] = ::studentUserName;

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

    if (responseObject["type"].toString() == "student_grade") {
        int grade = responseObject["grade"].toInt();
        QMessageBox::information(this, "نمره", "نمره شما: " + QString::number(grade));
    } else {
        QString errorMessage = responseObject["message"].toString();
        QMessageBox::critical(this, "خطا", errorMessage);
    }

    socket.disconnectFromHost();
}

