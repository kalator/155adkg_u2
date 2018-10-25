#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QtGui>

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPointF> points; //body
    QPolygonF ch; //kovexni obalka

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    QPolygonF getPolygon() {return ch;}
    void setCH(QPolygonF ch) {this->ch = ch;}
    std::vector<QPointF> getPoints() {return points;}
    void setPoints(std::vector<QPointF> &random_points) {this->points = random_points;}
    void clearCanvas();

signals:

public slots:
};

#endif // DRAW_H
