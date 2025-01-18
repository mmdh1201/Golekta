#include "mainwindow.h"
#include "application.h"
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

// void MainWindow::validatelogin(){
//     if(!validateCap()){return;}
//     QJsonObject user;
//     user["request"] = "check_user";
//     user["password"] = ui->PassLineEdit->text();
//     user["username"] = ui->UnameLineEdit->text();
//     QJsonDocument doc(user);
//     QByteArray data = doc.toJson();

//     QTcpSocket *socket = new QTcpSocket(this);
//     socket->connectToHost("127.0.0.1",12345);
//     if(socket->waitForConnected(3000)){
//         qDebug()<<"connected to the server.\n";
//         socket->write(data);
//         socket->flush();
//         if (socket->waitForReadyRead(6000)) {
//             QByteArray response = socket->readAll();
//             QJsonDocument docResponse = QJsonDocument::fromJson(response);
//             QJsonObject jsonResponse = docResponse.object();
//             QString messageType = jsonResponse["type"].toString();
//             QString message = jsonResponse["message"].toString();
//             if (messageType == "error") {
//                 this->ui->errorLabel->setText("*incorrect username or password");
//                 generateCaptcha();
//                 clearstuff();
//             } else if (messageType == "success") {
//                 QMessageBox::information(this, "Success", message);
//                 clearstuff();
//             }
//         }
//         }

//     // QByteArray data = file.readAll();
//     // file.close();

//     // QJsonDocument document = QJsonDocument::fromJson(data);
//     // //if the file was an object
//     // if (document.isObject()) {
//     //     QJsonObject users = document.object();
//     //     if(!users.contains(ui->UnameLineEdit->text())){
//     //         ui->errorLabel->setText("*incorrect username or password");
//     //         ui->errorLabel->setStyleSheet("color:rgb(255, 180, 180),font-size:10pt; ");
//     //         qDebug()<<"user not found\n";
//     //         generateCaptcha();
//     //         clearstuff();
//     //         return;
//     //     }
//     //     QJsonObject user = users.value(ui->UnameLineEdit->text()).toObject();
//     //     if(user.value("password").toString()==ui->PassLineEdit->text()){
//     //         QMessageBox::information(this,"congrats","You've logged in.");
//     //     }else{
//     //         ui->errorLabel->setText("*incorrect username or password");
//     //         ui->errorLabel->setStyleSheet("color:rgb(255, 180, 180),font-size:10pt; ");
//     //         qDebug()<<"user not found\n";
//     //         generateCaptcha();
//     //         clearstuff();
//     //         return;
//     //     }
//     // //if the file was an array
//     // } else if (document.isArray()) {
//     //     QJsonArray users = document.array();
//     //     for (int i = 0 ; i<users.size();i++){
//     //         QJsonObject obj = users.at(i).toObject();
//     //         if(obj.value("username")==ui->UnameLineEdit->text()){
//     //             if(obj.value("password")==ui->UnameLineEdit->text()){
//     //                 qDebug()<<"logged in.\n";
//     //                 clearstuff();
//     //                 return;
//     //             }
//     //         }
//     //     }
//     //     ui->errorLabel->setText("*incorrect username or password");
//     //     ui->errorLabel->setStyleSheet("color:rgb(255, 180, 180)");
//     //     clearstuff();
//     //     qDebug()<<"user not found\n";
//     }
void MainWindow::validatelogin() {
    if (!validateCap()) return;

    QJsonObject user;
    user["type"] = "check_user";
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

        if (messageType == "error") {
            ui->errorLabel->setText("*Incorrect username or password");
            generateCaptcha();
        } else if (messageType == "success") {
            QMessageBox::information(this, "Success", message);
            clearstuff();
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
