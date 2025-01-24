#ifndef STUDENTEXAM_H
#define STUDENTEXAM_H

#include <QDialog>

namespace Ui {
class StudentExam;
}

class StudentExam : public QDialog
{
    Q_OBJECT

public:
    explicit StudentExam(const QString &courseKey, const QString &studentUsername, QWidget *parent = nullptr);
    ~StudentExam();

private slots:
    void loadExamQuestion();
    void onSubmitButtonClicked();

private:
    Ui::StudentExam *ui;
    QString courseKey;
    QString studentUsername;
};

#endif // STUDENTEXAM_H
