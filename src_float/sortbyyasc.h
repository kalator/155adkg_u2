#ifndef SORTBYYASC_H
#define SORTBYYASC_H

#include <QtGui>

class SortByYAsc
{
public:
    SortByYAsc();

    //override (), sort by y, if y-es are same, sort by x, ascending
    bool operator()(QPointF &p1, QPointF &p2)
    {
        return (p1.y() < p2.y())||(p1.y()==p2.y() && p1.x() < p2.x());
    }
};

#endif // SORTBYYASC_H
