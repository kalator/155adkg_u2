#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include "algorithms.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_createCHButton_clicked();

    void on_generateButton_clicked();

    void on_clearButton_clicked();

    void onNextSent(int i);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
