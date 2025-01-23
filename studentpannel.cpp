#include "studentpannel.h"
#include "ui_studentpannel.h"
#include "application.h"
#include "global.h"
#include <QDate>

studentpannel::studentpannel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::studentpannel)
{
    ui->setupUi(this);
    ui->datelabel->setText(QDate::currentDate().toString());
        stdloadinfo();
}


studentpannel::~studentpannel()
{
    delete ui;
}
void studentpannel::stdloadinfo(){
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (socket.waitForConnected(3000)) {
        QJsonObject request;
        request["type"] = "get_student_info";
        request["username"] = stdusername;
        qDebug() << stdusername;
        QJsonDocument doc(request);
        socket.write(doc.toJson());
        socket.waitForBytesWritten();
        if (socket.waitForReadyRead(3000)) {
            QByteArray response = socket.readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject studentInfo = responseDoc.object();

            if (studentInfo["type"].toString() == "student_info") {
                stdemail = studentInfo["email"].toString();
                stdname = studentInfo["name"].toString();
                stdfaculty = studentInfo["faculty"].toString();
                stdcompletedUnits = studentInfo["completed_units"].toString();
                stdpassword = studentInfo["password"].toString();
                qDebug()<<stdemail;
            }
        }
    } else {
        qDebug() << "Error: " << socket.errorString();
    }

    socket.disconnectFromHost();
}

void studentpannel::showId(){
    ui->stdIDlabel->setText(app()->stdid);
    this->stdid = app()->stdid;
}


void studentpannel::on_statsbutton_2_clicked()
{
    this->close();
    app()->getstdstat()->show();
}

