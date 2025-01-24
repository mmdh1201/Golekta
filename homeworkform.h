#ifndef HOMEWORKFORM_H
#define HOMEWORKFORM_H

#include <QDialog>

namespace Ui {
class HomeworkForm;
}

class HomeworkForm : public QDialog
{
    Q_OBJECT

public:
    explicit HomeworkForm(const QString &courseName, const QString &professorName, QWidget *parent = nullptr);
    ~HomeworkForm();

private slots:
    void onSubmitButtonClicked();

private:
    Ui::HomeworkForm *ui;
    QString courseName;
    QString professorName;
    void sendHomeworkToServer(const QString &homeworkText);
};

#endif
