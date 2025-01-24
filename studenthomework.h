#ifndef STUDENTHOMEWORK_H
#define STUDENTHOMEWORK_H

#include <QDialog>

namespace Ui {
class StudentHomeWork;
}

class StudentHomeWork : public QDialog
{
    Q_OBJECT

public:
    explicit StudentHomeWork(const QString &courseKey, const QString &studentUsername, QWidget *parent);
    ~StudentHomeWork();

private slots:
    void loadHomeWorkQuestion();
    void onSubmitButtonClicked();
private:
    QString courseKey;
    QString studentUsername;
    Ui::StudentHomeWork *ui;
};

#endif // STUDENTHOMEWORK_H
