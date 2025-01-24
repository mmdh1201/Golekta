#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adminmainwindow.h"
#include "professormainwindow.h"
#include "studentmainwindow.h"
#include <QPainter>
#include <QPixmap>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void generateCap();

private slots:
    void onLoginButtonClicked();

private:
    Ui::MainWindow *ui;
    QString generateCaptcha();
    QString currentCaptcha;
    AdminMainWindow* ADMainWindow = nullptr;
    ProfessorMainWindow* PRMainWindow = nullptr;
    StudentMainWindow* STDMainWindow = nullptr;
    void validateCredentials(const QString &username, const QString &password, const QString &captcha);
};

#endif
