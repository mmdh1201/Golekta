#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QRandomGenerator>
#include <QPainter>
#include <QPixmap>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void validatelogin();


private slots:
    void on_UnameLineEdit_selectionChanged();

    void on_UnameLineEdit_editingFinished();

    void on_PassLineEdit_selectionChanged();

    void on_PassLineEdit_editingFinished();

    void on_UnameLineEdit_textChanged();

    void on_PassLineEdit_textChanged();

    void generateCaptcha();

    bool validateCap();

    void on_loginButton_clicked();

private:
    QString randomStr(int len){
        const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        QString result;
        for (int i = 0; i < len; ++i) {
            int index = QRandomGenerator::global()->bounded(chars.size());
            result.append(chars[index]);
        }
        return result;
    }

    void clearstuff(){
        //clear all line_edits
        ui->PassLineEdit->setText("");
        ui->UnameLineEdit->setText("");
        ui->errorLabel->setText("");
        ui->captchaline->setText("");
    };

    QString capTxt;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
