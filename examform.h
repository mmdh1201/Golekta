#ifndef EXAMFORM_H
#define EXAMFORM_H

#include <QDialog>

namespace Ui {
class ExamForm;
}

class ExamForm : public QDialog
{
    Q_OBJECT

public:
    explicit ExamForm(const QString &courseName, const QString &professorName, QWidget *parent = nullptr);
    ~ExamForm();

private slots:
    void onSubmitButtonClicked();

private:
    Ui::ExamForm *ui;
    QString courseName;
    QString professorName;
    void sendExamDetailsToServer(const QString &examQuestion, const QString &startingTime, const QString &endingTime);
};

#endif
