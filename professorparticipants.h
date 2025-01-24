#ifndef PROFESSORPARTICIPANTS_H
#define PROFESSORPARTICIPANTS_H

#include <QDialog>

namespace Ui {
class ProfessorParticipants;
}

class ProfessorParticipants : public QDialog
{
    Q_OBJECT

public:
    explicit ProfessorParticipants(const QString &courseKey, QWidget *parent = nullptr);
    ~ProfessorParticipants();

private:
    void loadParticipants();

    Ui::ProfessorParticipants *ui;
    QString courseKey;
};

#endif // PROFESSORPARTICIPANTS_H
