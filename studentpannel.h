#ifndef STUDENTPANNEL_H
#define STUDENTPANNEL_H

#include <QWidget>

namespace Ui {
class studentpannel;
}

class studentpannel : public QWidget
{
    Q_OBJECT

public:
    explicit studentpannel(QWidget *parent = nullptr);
    ~studentpannel();
    void showId();
    void stdloadinfo();

private slots:
    void on_statsbutton_2_clicked();

private:
    QString stdid;
    void showEvent(QShowEvent *event) override{
        QWidget::showEvent(event);
        this->showId();
    }
    Ui::studentpannel *ui;
};

#endif // STUDENTPANNEL_H
