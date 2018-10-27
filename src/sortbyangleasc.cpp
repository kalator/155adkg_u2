#include "sortbyangleasc.h"

SortByAngleAsc::SortByAngleAsc()
{

}

double SortByAngleAsc::getAngle(QPointF &p)
{
    double ux = -1000;
    double vx = p.x();
    double vy = p.y();

    double dot = ux*vx;

    double nu = fabs(ux*ux);
    double nv = sqrt(vx*vx + vy*vy);

    return acos(dot/(nu*nv))*180/M_PI;
}

double SortByAngleAsc::getDistance(QPointF &p)
{
    return sqrt(p.x()*p.x()+p.y()*p.y());
}
