#include "studentregisterform.h"
#include "ui_studentregisterform.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QJsonArray>
#include <QMessageBox>
//#include "global.h"

StudentRegisterForm::StudentRegisterForm(const QString &studentUsername, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentRegisterForm),
    studentUsername(studentUsername)
{
    ui->setupUi(this);
    loadCourses();
    connect(ui->submitButton, &QPushButton::clicked, this, &StudentRegisterForm::onSubmitButtonClicked);
    ui->courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);

}

StudentRegisterForm::~StudentRegisterForm()
{
    delete ui;
}

void StudentRegisterForm::loadCourses()
{
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
        return;
    }

    QJsonObject request;
    request["type"] = "get_courses";
    QJsonDocument requestDoc(request);
    socket.write(requestDoc.toJson());
    socket.waitForBytesWritten();

    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "خطا", "پاسخی از سرور دریافت نشد.");
        return;
    }

    QByteArray response = socket.readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(response);

    if (!responseDoc.isObject()) {
        QMessageBox::critical(this, "خطا", "فرمت پاسخ سرور نامعتبر است.");
        return;
    }

    QJsonObject responseObject = responseDoc.object();
    if (responseObject["type"].toString() != "course_list") {
        QMessageBox::critical(this, "خطا", "نوع پاسخ سرور نامعتبر است.");
        return;
    }
    \
    QJsonObject coursesObject = responseObject["courses"].toObject();
    int row = 0;
    ui->courseTable->setRowCount(coursesObject.keys().size());

    for (const QString &key : coursesObject.keys()) {
        QJsonObject course = coursesObject[key].toObject();

        ui->courseTable->setItem(row, 0, new QTableWidgetItem(course["course_id"].toString()));
        ui->courseTable->setItem(row, 1, new QTableWidgetItem(course["course_name"].toString()));
        ui->courseTable->setItem(row, 2, new QTableWidgetItem(course["course_days"].toString()));
        ui->courseTable->setItem(row, 3, new QTableWidgetItem(course["course_final_date"].toString()));
        ui->courseTable->setItem(row, 4, new QTableWidgetItem(course["course_location"].toString()));
        ui->courseTable->setItem(row, 5, new QTableWidgetItem(course["course_master_name"].toString()));
        ui->courseTable->setItem(row, 6, new QTableWidgetItem(course["course_time"].toString()));
        ui->courseTable->setItem(row, 7, new QTableWidgetItem(QString::number(course["course_capacity"].toInt())));

        ++row;
    }

    socket.disconnectFromHost();
}

void StudentRegisterForm::onSubmitButtonClicked()
{
    int currentRow = ui->courseTable->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "هشدار", "لطفاً یک درس را انتخاب کنید.");
        return;
    }

    QTableWidgetItem *courseIdItem = ui->courseTable->item(currentRow, 0);
    if (!courseIdItem) {
        QMessageBox::critical(this, "خطا", "آی‌دی درس یافت نشد.");
        return;
    }

    QString selectedCourseId = courseIdItem->text();

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
        return;
    }
    QJsonObject request;
    request["type"] = "student_enroll_in_course";
    request["course_id"] = selectedCourseId;
    request["student_username"] = studentUsername;

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

    QString responseType = responseObject["type"].toString();
    QString message = responseObject["message"].toString();

    if (responseType == "error") {
        QMessageBox::critical(this, "خطا", message);
    } else if (responseType == "success") {
        QMessageBox::information(this, "موفقیت", message);
    }

    socket.disconnectFromHost();
}
