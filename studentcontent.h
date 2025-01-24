#ifndef STUDENTCONTENT_H
#define STUDENTCONTENT_H

#include <QWidget>

namespace Ui {
class StudentContent;
}

class StudentContent : public QWidget
{
    Q_OBJECT

public:
    explicit StudentContent(QWidget *parent = nullptr);
    ~StudentContent();

private:
    Ui::StudentContent *ui;
};

#endif // STUDENTCONTENT_H
