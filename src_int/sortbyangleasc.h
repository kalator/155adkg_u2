#ifndef SORTBYANGLEASC_H
#define SORTBYANGLEASC_H

#include <QtGui>

class SortByAngleAsc
{
private:
    QPoint q;

public:
    SortByAngleAsc(QPoint &q);

    double getAngle(QPoint &p);
    double getDistance(QPoint &p);

    //override (), sort by angles and if angles are the same, sort by distance, ascending
    bool operator()(QPoint &p1, QPoint &p2)
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
