#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QtGui>

#include "algorithms.h"

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPointF> points; //points
    QPolygonF ch; //convex hull
    QPolygonF rect; //minimal rectangle
    QLineF direction; //main direction

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    QPolygonF getPolygon() {return ch;}
    void setCH(QPolygonF ch);
    std::vector<QPointF> getPoints() {return points;}
    void setPoints(std::vector<QPointF> &random_points) {this->points = random_points;}
    void setRect(bool draw_dir_line);
    void clearCanvas();

signals:

public slots:
};

#endif // DRAW_H
