#include "addcourseform.h"
#include "ui_addcourseform.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QComboBox>
#include <QJsonArray>

AddCourseForm::AddCourseForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddCourseForm)
{
    ui->setupUi(this);
    loadInstructors();
}

AddCourseForm::~AddCourseForm()
{
    delete ui;
}

void AddCourseForm::on_submitButton_clicked()
{
    QTcpSocket *socket;
    QString courseName = ui->courseNameLineEdit->text();
    int units = ui->unitsLineEdit->text().toInt();
    QString courseNumber = ui->courseNumberLineEdit->text();
    QString courseGroup = ui->courseGroupLineEdit->text();
    int courseCapacity = ui->courseCapacityLineEdit->text().toInt();
    QString courseLocation = ui->courseLocationLineEdit->text();
    QString courseDays = ui->courseDaysLineEdit->text();
    QString courseTime = ui->courseTimeLineEdit->text();
    QString courseFinalDate = ui->courseFinalDateLineEdit->text();
    QString courseMasterName = ui->courseMasterComboBox->currentText();

    if (courseName.isEmpty() || courseNumber.isEmpty() || courseGroup.isEmpty() || courseMasterName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled.");
        return;
    }

    QJsonObject courseData;
    courseData["type"] = "enroll_in_course";
    courseData["course_id"] = courseNumber + "_" + courseGroup;
    courseData["course_name"] = courseName;
    courseData["units"] = units;
    courseData["course_number"] = courseNumber;
    courseData["course_group"] = courseGroup;
    courseData["course_capacity"] = courseCapacity;
    courseData["course_location"] = courseLocation;
    courseData["course_days"] = courseDays;
    courseData["course_time"] = courseTime;
    courseData["course_final_date"] = courseFinalDate;
    courseData["course_master_name"] = courseMasterName;

    QJsonDocument doc(courseData);
    QByteArray jsonData = doc.toJson();

    socket = new QTcpSocket(this);

    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected to server";

        socket->write(jsonData);
        socket->flush();

        if (socket->waitForReadyRead(3000)) {
            QByteArray response = socket->readAll();


            QJsonDocument docResponse = QJsonDocument::fromJson(response);
            QJsonObject jsonResponse = docResponse.object();

            QString messageType = jsonResponse["type"].toString();
            QString message = jsonResponse["message"].toString();

            if (messageType == "error") {
                QMessageBox::critical(this, "Error", message);
            } else if (messageType == "success") {
                QMessageBox::information(this, "Success", message);
            }
        }

        socket->disconnectFromHost();
    } else {
        qDebug() << "Error: " << socket->errorString();
        QMessageBox::critical(this, "Connection Error", "Could not connect to server.");
    }

    socket->close();
}

void AddCourseForm::loadInstructors()
{
    QTcpSocket* socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected to server";

        QJsonObject request;
        request["type"] = "get_instructors";
        QJsonDocument requestDoc(request);
        QByteArray requestData = requestDoc.toJson();
        socket->write(requestData);
        socket->flush();

        if (socket->waitForReadyRead(3000)) {
            QByteArray response = socket->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonResponse = responseDoc.object();

            if (jsonResponse["type"].toString() == "instructors_list") {
                QJsonArray instructorsArray = jsonResponse["instructors"].toArray();
                QStringList instructorsList;

                for (int i = 0; i < instructorsArray.size(); ++i) {
                    instructorsList.append(instructorsArray[i].toString());
                }

                ui->courseMasterComboBox->clear();
                ui->courseMasterComboBox->addItems(instructorsList);
            }
        }
    } else {
        qDebug() << "Error: " << socket->errorString();
        QMessageBox::critical(this, "Connection Error", "Could not connect to server.");
    }

    socket->close();
}
