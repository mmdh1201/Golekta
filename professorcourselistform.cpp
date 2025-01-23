#include "professorcourselistform.h"
#include "ui_professorcourselistform.h"
#include "coursemanagementform.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QJsonArray>
#include "application.h"

ProfessorCourseListForm::ProfessorCourseListForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfessorCourseListForm)
{
    ui->setupUi(this);
    loadCourses();
    connect(ui->courseListWidget, &QListWidget::itemClicked, this, &ProfessorCourseListForm::onCourseSelected);
}

ProfessorCourseListForm::~ProfessorCourseListForm()
{
    delete ui;
}


void ProfessorCourseListForm::setprofname(){professorName = app()->profName;}
void ProfessorCourseListForm::loadCourses()
{
    QTcpSocket* socket;

    if (professorName.isEmpty()) {
        QMessageBox::warning(this, "Error", "No professor selected.");
        return;
    }

    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected to server";

        QJsonObject request;
        request["type"] = "get_course_list";
        request["professor_name"] = professorName;
        QJsonDocument requestDoc(request);
        QByteArray requestData = requestDoc.toJson();
        socket->write(requestData);
        socket->flush();

        if (socket->waitForReadyRead(3000)) {
            QByteArray response = socket->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonResponse = responseDoc.object();

            if (jsonResponse["type"].toString() == "course_list") {
                QJsonArray coursesArray = jsonResponse["courses"].toArray();
                QStringList courseList;

                if (coursesArray.isEmpty()) {
                    QMessageBox::information(this, "No Courses", "No courses found for this professor.");
                } else {
                    for (int i = 0; i < coursesArray.size(); ++i) {
                        courseList.append(coursesArray[i].toString());
                    }
                    ui->courseListWidget->clear();
                    ui->courseListWidget->addItems(courseList);
                }
            }
        }
    } else {
        qDebug() << "Error: " << socket->errorString();
        QMessageBox::critical(this, "Connection Error", "Could not connect to server.");
    }

    socket->close();
}


void ProfessorCourseListForm::onCourseSelected(QListWidgetItem *item)
{
    QString courseName = item->text();
    if (!CMForm) {
        CMForm = new CourseManagementForm(courseName, this);
        connect(CMForm, &QWidget::destroyed, this, [&]() {
            CMForm = nullptr;
        });
    }
    CMForm->show();
}
