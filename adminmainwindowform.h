#ifndef ADMINMAINWINDOWFORM_H
#define ADMINMAINWINDOWFORM_H

#include <QWidget>

namespace Ui {
class AdminMainWindowForm;
}

class AdminMainWindowForm : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMainWindowForm(QWidget *parent = nullptr);
    ~AdminMainWindowForm();

private:
    Ui::AdminMainWindowForm *ui;
};

#endif // ADMINMAINWINDOWFORM_H
