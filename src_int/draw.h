#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QtGui>

#include "algorithms.h"

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPoint> points; //points
    QPolygon ch; //convex hull
    QPolygon rect; //minimal rectangle
    QLine direction; //main direction

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    double setCH(std::string &selected_algorithm);
    void setPoints(QSize &canvas_size, int count, std::string &shape);
    void setRect(bool draw_dir_line);
    void clearCanvas();

signals:

public slots:
};

#endif // DRAW_H
