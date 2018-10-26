#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <string>
#include <cmath>
#include <ctime>

#include "sortbyxasc.h"
#include "sortbyyasc.h"

typedef enum{
    LEFT = 0,
    RIGHT = 1,
    ON = 2
} TPositon; //vyctovy typ

class Algorithms : public QObject
{
    Q_OBJECT
public:

    Algorithms();
    static TPositon getPointLinePosition(QPointF &q, QPointF &a, QPointF &b);
    static double getTwoVectorsAngle(QPointF &p1, QPointF &p2, QPointF &p3, QPointF &p4);
    static QPolygonF jarvisScanCH(std::vector<QPointF> &points);
    static std::vector<QPointF> generatePoints(QSize &canvas_size, int point_count, std::string shape);

signals:
    void static next(int i);
};

#endif // ALGORITHMS_H
