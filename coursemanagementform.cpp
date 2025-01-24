#include "coursemanagementform.h"
#include <QMessageBox>
#include "ui_coursemanagementform.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QJsonArray>
#include "global.h"
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

CourseManagementForm::CourseManagementForm(const QString &courseName, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CourseManagementForm)
    , courseName(courseName)
{
    ui->setupUi(this);

    connect(ui->participantsButton,
            &QPushButton::clicked,
            this,
            &CourseManagementForm::onParticipantsButtonClicked);
    connect(ui->contentButton,
            &QPushButton::clicked,
            this,
            &CourseManagementForm::onContentButtonClicked);
    connect(ui->examButton, &QPushButton::clicked, this, &CourseManagementForm::onExamButtonClicked);
    connect(ui->assignmentButton,
            &QPushButton::clicked,
            this,
            &CourseManagementForm::onAssignmentButtonClicked);
    connect(ui->gradesButton,
            &QPushButton::clicked,
            this,
            &CourseManagementForm::onGradesButtonClicked);
}

CourseManagementForm::~CourseManagementForm()
{
    delete ui;
}

void CourseManagementForm::onParticipantsButtonClicked()
{
    if (!PARForm) {
        PARForm = new ProfessorParticipants(courseKey, this);
        PARForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(PARForm, &QWidget::destroyed, this, [&]() {
            PARForm = nullptr;
        });
    }
    PARForm->show();
}

void CourseManagementForm::onContentButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Content File", "", "All Files (*.*)");

    if (!fileName.isEmpty()) {
        uploadFile(fileName, "upload_content");
    }
}

void CourseManagementForm::onExamButtonClicked()
{
    if (!EXForm) {
        EXForm = new ExamForm(courseName, professorName, this);
        EXForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(EXForm, &QWidget::destroyed, this, [&]() {
            EXForm = nullptr;
        });
    }
    EXForm->show();
}

void CourseManagementForm::onAssignmentButtonClicked()
{
    if (!HWForm) {
        HWForm = new HomeworkForm(courseName, professorName, this);
        HWForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(HWForm, &QWidget::destroyed, this, [&]() {
            HWForm = nullptr;
        });
    }
    HWForm->show();
}

void CourseManagementForm::onGradesButtonClicked()
{
    if (!GRForm) {
        GRForm = new ProfessorGrade(courseKey, this);
        GRForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(GRForm, &QWidget::destroyed, this, [&]() {
            GRForm = nullptr;
        });
    }
    GRForm->show();
}


void CourseManagementForm::uploadFile(const QString &fileName, const QString &requestType)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected to server";
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Error", "Unable to open the file.");
            return;
        }
        QJsonObject request;
        request["type"] = requestType;
        request["course_name"] = courseName;
        request["professor_name"] = professorName;
        request["file_name"] = QFileInfo(fileName).fileName();
        QJsonDocument requestDoc(request);
        QByteArray requestData = requestDoc.toJson();

        socket->write(requestData);
        socket->flush();

        socket->write(file.readAll());
        socket->flush();

        if (socket->waitForReadyRead(3000)) {
            QByteArray response = socket->readAll();

            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject responseObj = responseDoc.object();
            qDebug()<<responseObj["message"];
            if (responseObj["type"].toString() == "success") {
                QMessageBox::information(this, "Success", "Content uploaded successfully.");
            } else {
                QMessageBox::critical(this, "Error", "Failed to upload content.");
            }
        }
    } else {
        qDebug() << "Error: " << socket->errorString();
        QMessageBox::critical(this, "Connection Error", "Could not connect to server.");
    }

    socket->close();
}
