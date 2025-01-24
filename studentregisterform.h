#ifndef STUDENTREGISTER_H
#define STUDENTREGISTER_H

#include <QDialog>

namespace Ui {
class StudentRegisterForm;
}

class StudentRegisterForm : public QDialog
{
    Q_OBJECT

public:
    explicit StudentRegisterForm(const QString &studentUsername, QWidget *parent = nullptr);
    ~StudentRegisterForm();

private slots:
    void loadCourses();
    void onSubmitButtonClicked();

private:
    Ui::StudentRegisterForm *ui;
    QString studentUsername;
};

#endif // STUDENTREGISTER_H
