#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "professormainwindow.h"
#include "adminmainwindow.h"
#include "global.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPixmap>
#include <QRandomGenerator>
#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentCaptcha = generateCaptcha();
    generateCap();

    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::generateCaptcha()
{
    QString captcha;
    const QString characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < 6; ++i) {
        captcha += characters[QRandomGenerator::global()->bounded(characters.size())];
    }
    return captcha;
}
void MainWindow::generateCap(){
    currentCaptcha = generateCaptcha();
    QPixmap pixmap(250,50);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    for (int i = 0; i < currentCaptcha.size(); ++i) {
        painter.setPen(QColor(QRandomGenerator::global()->bounded(256),
                              QRandomGenerator::global()->bounded(256),
                              QRandomGenerator::global()->bounded(256)));
        int x = 20 + i * 30;
        int y = 30 + QRandomGenerator::global()->bounded(-10, 10);
        painter.drawText(x, y, QString(currentCaptcha[i]));
        ui->captchaImage->setPixmap(pixmap);
    }
    painter.end();
}

void MainWindow::onLoginButtonClicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text().trimmed();
    QString captcha = ui->captchaLineEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || captcha.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً تمام فیلدها را پر کنید.");
        return;
    }

    if (captcha != currentCaptcha) {
        QMessageBox::critical(this, "خطا", "کپچا اشتباه است.");
        currentCaptcha = generateCaptcha();
        ui->captchaImage->setText(currentCaptcha);
        return;
    }

    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected(3000)) {
        QJsonObject request;
        request["type"] = "login_request";
        request["username"] = username;
        request["password"] = password;
        professorUserName = username;
        studentUserName = username;

        QJsonDocument requestDoc(request);
        socket->write(requestDoc.toJson());
        socket->flush();

        if (socket->waitForReadyRead(3000)) {
            QByteArray response = socket->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject responseObj = responseDoc.object();

            QString messageType = responseObj["type"].toString();
            QString message = responseObj["message"].toString();

            if (messageType == "error") {
                QMessageBox::critical(this, "خطا", message);
            } else if (messageType == "success") {
                QString role = responseObj["role"].toString();
                QMessageBox::information(this, "موفقیت", message);

                if (role == "master") {
                    if (!PRMainWindow) {
                        PRMainWindow = new ProfessorMainWindow(this);
                        PRMainWindow->setAttribute(Qt::WA_DeleteOnClose);
                        connect(PRMainWindow, &QWidget::destroyed, this, [&]() {
                            PRMainWindow = nullptr;
                        });
                    }
                    PRMainWindow->show();
                    this->hide();
                } else if (role == "admin") {
                    if (!ADMainWindow) {
                        ADMainWindow = new AdminMainWindow(this);
                        ADMainWindow->setAttribute(Qt::WA_DeleteOnClose);
                        connect(ADMainWindow, &QWidget::destroyed, this, [&]() {
                            ADMainWindow = nullptr;
                        });
                    }
                    ADMainWindow->show();
                    this->hide();
                } else if (role == "student") {
                    if (!STDMainWindow) {
                        STDMainWindow = new StudentMainWindow(this);
                        STDMainWindow->setAttribute(Qt::WA_DeleteOnClose);
                        connect(STDMainWindow, &QWidget::destroyed, this, [&]() {
                            STDMainWindow = nullptr;
                        });
                    }
                    STDMainWindow->show();
                    this->hide();
                }else {
                    QMessageBox::critical(this, "خطا", "نقش نامعتبر است.");
                }
            }
        }
    } else {
        QMessageBox::critical(this, "خطا", "اتصال به سرور ممکن نیست.");
    }

    socket->close();
}
