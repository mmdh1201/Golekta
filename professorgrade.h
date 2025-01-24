#ifndef PROFESSORGRADE_H
#define PROFESSORGRADE_H

#include <QDialog>

namespace Ui {
class ProfessorGrade;
}

class ProfessorGrade : public QDialog
{
    Q_OBJECT

public:
    explicit ProfessorGrade(const QString &courseKey, QWidget *parent = nullptr);
    ~ProfessorGrade();

private slots:
    void loadParticipants();
    void onSubmitButtonClicked();

private:
    Ui::ProfessorGrade *ui;
    QString courseKey;
};

#endif // PROFESSORGRADE_H
