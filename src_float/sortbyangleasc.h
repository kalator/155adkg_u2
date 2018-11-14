#ifndef SORTBYANGLEASC_H
#define SORTBYANGLEASC_H

#include <QtGui>

class SortByAngleAsc
{
private:
    QPointF q;

public:
    SortByAngleAsc(QPointF &q);

    double getAngle(QPointF &p);
    double getDistance(QPointF &p);

    //override (), sort by angles and if angles are the same, sort by distance, ascending
    bool operator()(QPointF &p1, QPointF &p2)
    {
        if(fabs(getAngle(p1)-getAngle(p2)) < 10e-6)
        {
            if(getDistance(p1) < getDistance(p2))
                return 1;

            else
                return 0;
        }

        else if (getAngle(p1) < getAngle(p2))
            return 1;

        else
            return 0;
    }
};

#endif // SORTBYANGLEASC_H
