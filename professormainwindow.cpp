#include "professormainwindow.h"
#include "ui_professormainwindow.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include "global.h"

ProfessorMainWindow::ProfessorMainWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfessorMainWindow)
{
    ui->setupUi(this);
    connect(ui->showInfoButton, &QPushButton::clicked, this, &ProfessorMainWindow::showInfo);
    connect(ui->courseListButton, &QPushButton::clicked, this, &ProfessorMainWindow::showCourseList);
    globalLoadProfessorInfo();
}

ProfessorMainWindow::~ProfessorMainWindow()
{
    delete ui;
}

void ProfessorMainWindow::showInfo()
{
    if (!SIForm) {
        SIForm = new ProfessorInfoForm(this);
        connect(SIForm, &QWidget::destroyed, this, [&]() {
            SIForm = nullptr;
        });
    }
    SIForm->show();
}

void ProfessorMainWindow::showCourseList()
{
    if (!CLForm) {
        CLForm = new ProfessorCourseListForm(this);
        connect(CLForm, &QWidget::destroyed, this, [&]() {
            CLForm = nullptr;
        });
    }
    CLForm->show();
}

void ProfessorMainWindow::globalLoadProfessorInfo()
{
    QString username = ::professorUserName;//NOTE:needs an event to initialize
    qDebug()<< professorUserName;

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (socket.waitForConnected(3000)) {
        QJsonObject request;
        request["type"] = "get_professor_info";
        request["username"] = username;
        QJsonDocument doc(request);
        socket.write(doc.toJson());
        socket.waitForBytesWritten();

        if (socket.waitForReadyRead(3000)) {
            QByteArray response = socket.readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject professorInfo = responseDoc.object();

            if (professorInfo["type"].toString() == "professor_info") {

                professorEmail = professorInfo["email"].toString();
                professorName = professorInfo["name"].toString();
                professorFaculty = professorInfo["faculty"].toString();
                professorOfficeNumber = professorInfo["office_number"].toString();
                professorOfficePhoneNumber = professorInfo["office_phone"].toString();
            }
        }
    } else {
        qDebug() << "Error: " << socket.errorString();
    }

    socket.disconnectFromHost();
}
