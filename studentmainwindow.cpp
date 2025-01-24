#include "studentmainwindow.h"
#include "ui_studentmainwindow.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QJsonArray>
#include "global.h"

StudentMainWindow::StudentMainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentMainWindow)
{
    ui->setupUi(this);
    connect(ui->registerTermButton, &QPushButton::clicked, this, &StudentMainWindow::onRegisterTermClicked);
    connect(ui->viewInfoButton, &QPushButton::clicked, this, &StudentMainWindow::onViewInfoClicked);
    connect(ui->removeCourseButton, &QPushButton::clicked, this, &StudentMainWindow::onRemoveCourseClicked);
    connect(ui->courseListWidget, &QListWidget::itemClicked, this, &StudentMainWindow::onCourseItemClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &StudentMainWindow::refreshCourseList);
    ui->courseListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    refreshCourseList();
    globalLoadStudentInfo();
}

StudentMainWindow::~StudentMainWindow()
{
    delete ui;
}

void StudentMainWindow::onRegisterTermClicked()
{
    if (!STDRForm) {
        STDRForm = new StudentRegisterForm(::studentUserName, this);
        STDRForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(STDRForm, &QWidget::destroyed, this, [&]() {
            STDRForm = nullptr;
        });
    }
    STDRForm->show();
}

void StudentMainWindow::onViewInfoClicked()
{
    if (!STDSIForm) {
        STDSIForm = new StudentShowInfoForm(this);
        STDSIForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(STDSIForm, &QWidget::destroyed, this, [&]() {
            STDSIForm = nullptr;
        });
    }
    STDSIForm->show();
}

void StudentMainWindow::onRemoveCourseClicked()
{
    QListWidgetItem *currentItem = ui->courseListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "خطا", "لطفاً یک درس را انتخاب کنید.");
        return;
    }

    QString courseKey = currentItem->text();

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
        return;
    }

    QJsonObject request;
    request["type"] = "remove_student_from_course";
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

    if (responseObject["type"].toString() == "success") {
        QMessageBox::information(this, "موفقیت", "درس با موفقیت حذف شد.");
        delete currentItem;
    } else {
        QString errorMessage = responseObject["message"].toString();
        QMessageBox::critical(this, "خطا", errorMessage);
    }

    socket.disconnectFromHost();
}

void StudentMainWindow::globalLoadStudentInfo()
{
    QString username = ::studentUserName;
    // qDebug()<<professorUserName;

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (socket.waitForConnected(3000)) {
        QJsonObject request;
        request["type"] = "get_student_info";
        request["username"] = username;
        QJsonDocument doc(request);
        socket.write(doc.toJson());
        socket.waitForBytesWritten();

        if (socket.waitForReadyRead(3000)) {
            QByteArray response = socket.readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject studentInfo = responseDoc.object();

            if (studentInfo["type"].toString() == "student_info") {

                studentEmail = studentInfo["email"].toString();
                studentName = studentInfo["name"].toString();
                studentFaculty = studentInfo["faculty"].toString();
                studentPassedUnits = studentInfo["completed_units"].toString();
                studentPassword = studentInfo["password"].toString();
                // qDebug()<<studentPassedUnits;
            }
        }
    } else {
        qDebug() << "Error: " << socket.errorString();
    }

    socket.disconnectFromHost();
}


void StudentMainWindow::refreshCourseList()
{
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
        return;
    }

    QJsonObject request;
    request["type"] = "get_registered_courses";
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

    if (responseObject["type"].toString() != "registered_courses") {
        QMessageBox::critical(this, "خطا", "نوع پاسخ سرور نامعتبر است.");
        return;
    }

    ui->courseListWidget->clear();

    QJsonArray coursesArray = responseObject["courses"].toArray();
    for (const QJsonValue &value : coursesArray) {
        QJsonObject courseInfo = value.toObject();
        QString courseKey = courseInfo["course_key"].toString();
        ::courseKey = courseKey;
        int grade = courseInfo["grade"].toInt();
        ::studentGrade = grade;

        QString courseItem = courseKey;
        ui->courseListWidget->addItem(courseItem);
    }

    socket.disconnectFromHost();
}



void StudentMainWindow::onCourseItemClicked(QListWidgetItem *item)
{

    QString courseKey = item->text();
    QStringList parts = courseKey.split('_');
    qDebug()<<courseKey;
    ::courseKey = courseKey;
    qDebug()<<::courseKey;
    if (parts.size() < 2) {
        QMessageBox::critical(this, "خطا", "اطلاعات درس نامعتبر است.");
        return;
    }

    QString courseName = parts[0];
    QString courseMaster = parts[1];

    if (!STDCForm) {
        STDCForm = new StudentCourseInfo(courseName, courseMaster, this);
        STDCForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(STDCForm, &QWidget::destroyed, this, [&]() {
            STDCForm = nullptr;
        });
    }
    STDCForm->show();
}

