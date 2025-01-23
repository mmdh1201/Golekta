#include "mainwindow.h"
#include "application.h"
#include "global.h"
#include "studentpannel.h"
#include "adminmainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QTcpSocket>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    generateCaptcha();
    connect(ui->loginButton,&QPushButton::clicked,this,&MainWindow::validatelogin);
    connect(ui->refreshcapBtton,&QPushButton::clicked,this,&MainWindow::generateCaptcha);
    connect(ui->toggleButton,&QPushButton::clicked,this,[=](){
        if(ui->PassLineEdit->echoMode()==QLineEdit::Password){
            ui->PassLineEdit->setEchoMode(QLineEdit::Normal);
        }else{
            ui->PassLineEdit->setEchoMode(QLineEdit::Password);
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::generateCaptcha(){
    capTxt = randomStr(5);
    QPixmap pixmap(250,50);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    for (int i = 0; i < capTxt.size(); ++i) {
        painter.setPen(QColor(QRandomGenerator::global()->bounded(256),
                              QRandomGenerator::global()->bounded(256),
                              QRandomGenerator::global()->bounded(256)));
        int x = 20 + i * 30;
        int y = 30 + QRandomGenerator::global()->bounded(-10, 10);
        painter.drawText(x, y, QString(capTxt[i]));
        ui->capLabel->setPixmap(pixmap);
    }
    painter.end();
}
bool MainWindow::validateCap(){
    if (ui->captchaline->text() != capTxt){
        QMessageBox *msg = new QMessageBox(QMessageBox::Warning, "Warning", "Invalid CAPTCHA. Please try again.", QMessageBox::Ok);
        msg->setStyleSheet("background-color: white; color: black;");
        generateCaptcha();
        msg->exec();
        return false;
    }
    return true;
}
void MainWindow::validatelogin() {
    // if (!validateCap()) return;

    QJsonObject user;
    user["type"] = "login_request";
    user["password"] = ui->PassLineEdit->text();
    user["username"] = ui->UnameLineEdit->text();
    QJsonDocument doc(user);
    QByteArray data = doc.toJson();

    QTcpSocket *socket = new QTcpSocket(this); // Parent ensures cleanup

    connect(socket, &QTcpSocket::connected, [socket, data]() {
        qDebug() << "Connected to server.";
        socket->write(data);
        socket->flush();
    });

    connect(socket, &QTcpSocket::readyRead, [this, socket]() {
        QByteArray response = socket->readAll();
        QJsonDocument docResponse = QJsonDocument::fromJson(response);
        if (docResponse.isNull()) {
            qDebug() << "Invalid JSON received.";
            return;
        }

        QJsonObject jsonResponse = docResponse.object();
        QString messageType = jsonResponse["type"].toString();
        QString message = jsonResponse["message"].toString();
        QString role = jsonResponse["role"].toString();

        if (messageType == "error") {
            ui->errorLabel->setText(message);
            generateCaptcha();
        } else if (messageType == "success") {
            if(role == "student"){
                stdusername = ui->UnameLineEdit->text();
                this->close();
                app()->getpannel()->show();
                clearstuff();
            }else if(role == "master"){
                app()->profName = ui->UnameLineEdit->text();
                this->close();
                app()->getmainprof()->show();
                clearstuff();
            }else if(role == "admin"){
                this->close();
                app()->getadmin()->show();
                clearstuff();
            }
        }
    });

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    socket->connectToHost("127.0.0.1", 12345);
    if (!socket->waitForConnected(3000)) {
        qDebug() << "Failed to connect to server.";
        socket->deleteLater();
    }
}


void MainWindow::on_loginButton_clicked()
{
    app()->stdid = ui->UnameLineEdit->text();
}
void MainWindow::on_UnameLineEdit_selectionChanged()
{
    ui->UnameLineEdit->setStyleSheet("background-color: white; color:black;border : none;border-bottom :  2px solid black");
}
void MainWindow::on_UnameLineEdit_editingFinished()
{
    ui->UnameLineEdit->setStyleSheet("border : none;border-bottom :  2px solid #C5FF95;color : #fff;font-size: 15px;");
}
void MainWindow::on_PassLineEdit_selectionChanged()
{
    ui->PassLineEdit->setStyleSheet("background-color: white; color:black;border : none;border-bottom :  2px solid black");
}
void MainWindow::on_PassLineEdit_editingFinished()
{
    ui->PassLineEdit->setStyleSheet("border : none;border-bottom :  2px solid #C5FF95;color : #fff;font-size: 15px;");
}
void MainWindow::on_UnameLineEdit_textChanged()
{
    ui->UnameLineEdit->setStyleSheet("background-color: white; color:black;border : none;border-bottom :  2px solid black");
}
void MainWindow::on_PassLineEdit_textChanged()
{
    ui->PassLineEdit->setStyleSheet("background-color: white; color:black;border : none;border-bottom :  2px solid black");
}

