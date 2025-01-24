#ifndef PROFESSORMAINWINDOW_H
#define PROFESSORMAINWINDOW_H

#include <QDialog>
#include "professorcourselistform.h"
#include "professorinfoform.h"

namespace Ui {
class ProfessorMainWindow;
}

class ProfessorMainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProfessorMainWindow(QWidget *parent = nullptr);
    ~ProfessorMainWindow();
private slots:
    void showInfo();
    void showCourseList();
    void globalLoadProfessorInfo();

private:
    Ui::ProfessorMainWindow *ui;
    ProfessorCourseListForm* CLForm = nullptr;
    ProfessorInfoForm* SIForm = nullptr;
};

#endif
