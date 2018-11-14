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
    static TPositon getPointLinePosition(QPoint &q, QPoint &a, QPoint &b);
    static double getTwoVectorsAngle(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4);
    static double getPointLineDistance(QPoint &q, QPoint &a, QPoint &b);

    //override
    static void rotateByAngle(std::vector<QPoint> &points, double angle);
    static void rotateByAngle(QPolygon &points, double angle);
    static void rotateByAngle(QLine &points, double angle);

    static double getDistance(QPoint &a, QPoint &b);

    static QPolygon jarvisScanCH(std::vector<QPoint> &points);
    static QPolygon grahamScanCH(std::vector<QPoint> &points);
    static QPolygon quickHullCH(std::vector<QPoint> &points);
    static void quickHullLocal(int s, int e, std::vector<QPoint> &points, QPolygon &poly_ch);
    static QPolygon sweepLineCH(std::vector<QPoint> &points);
    static std::vector<QPoint> generatePoints(QSize &canvas_size, int point_count, std::string shape);
    static void minimalRectangle(QPolygon &poly_ch, QPolygon &minimal_rectangle, QLine &direction, bool compute_dir_line);


};

#endif // ALGORITHMS_H
