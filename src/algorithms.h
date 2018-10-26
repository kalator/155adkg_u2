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

class Algorithms
{
public:

    Algorithms();
    static TPositon getPointLinePosition(QPointF &q, QPointF &a, QPointF &b);
    static double getTwoVectorsAngle(QPointF &p1, QPointF &p2, QPointF &p3, QPointF &p4);
    static QPolygonF jarvisScanCH(std::vector<QPointF> &points);
    static std::vector<QPointF> generatePoints(QSize &canvas_size, int point_count, std::string shape);
    static QPolygonF minimalRectangle(QPolygonF &poly_ch);
    static void rotateByAngle(std::vector<QPointF> &points, double angle);
    static void rotateByAngle(QPolygonF &points, double angle);
    static double getDistance(QPointF &a, QPointF &b);
};

#endif // ALGORITHMS_H
