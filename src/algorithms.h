#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <string>
#include <cmath>
#include <ctime>
#include <random>

#include "sortbyxasc.h"
#include "sortbyyasc.h"
#include "sortbyangleasc.h"

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
    static double getPointLineDistance(QPointF &q, QPointF &a, QPointF &b);
    static QPolygonF jarvisScanCH(std::vector<QPointF> &points);
    static QPolygonF grahamScanCH(std::vector<QPointF> &points);
    static QPolygonF quickHullCH(std::vector<QPointF> &points);
    static void quickHullLocal(int s, int e, std::vector<QPointF> &points, QPolygonF &poly_ch);
    static QPolygonF sweepLineCH(std::vector<QPointF> &points);
    static std::vector<QPointF> generatePoints(QSizeF &canvas_size, int point_count, std::string shape);
    static void minimalRectangle(QPolygonF &poly_ch, QPolygonF &minimal_rectangle, QLineF &direction, bool compute_dir_line);

    //override
    static void rotateByAngle(std::vector<QPointF> &points, double angle);
    static void rotateByAngle(QPolygonF &points, double angle);
    static void rotateByAngle(QLineF &points, double angle);

    static double getDistance(QPointF &a, QPointF &b);
};

#endif // ALGORITHMS_H
