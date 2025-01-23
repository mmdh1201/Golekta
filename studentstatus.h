#ifndef STUDENTSTATUS_H
#define STUDENTSTATUS_H

#include <QDialog>

namespace Ui {
class studentstatus;
}

class studentstatus : public QDialog
{
    Q_OBJECT

public:
    explicit studentstatus(QWidget *parent = nullptr);
    ~studentstatus();
    void loadinfo();

private:
    Ui::studentstatus *ui;
};

#endif // STUDENTSTATUS_H
