#include "sortbyangleasc.h"

SortByAngleAsc::SortByAngleAsc(QPointF &q)
{
    this->q=q;
}

double SortByAngleAsc::getAngle(QPointF &p)
{
    double ux = -100;
    double uy = 0;
    double vx = p.x() - q.x();
    double vy = p.y() - q.y();

    double dot = ux*vx + uy*vy;

    double nu = sqrt(ux*ux + uy*uy);
    double nv = sqrt(vx*vx + vy*vy);

    return acos(dot/(nu*nv))*180/M_PI;
}

double SortByAngleAsc::getDistance(QPointF &p)
{
    return sqrt((p.x()-q.x())*(p.x()-q.x())+(p.y()-q.y())*(p.y()-q.y()));
}
