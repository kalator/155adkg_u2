#ifndef SORTBYYASC_H
#define SORTBYYASC_H

#include <QtGui>

class SortByYAsc
{
public:
    SortByYAsc();

    bool operator()(QPointF &p1, QPointF &p2)
    {
        return (p1.y() < p2.y())||(p1.y()==p2.y() && p1.x() < p2.x());
    }
};

#endif // SORTBYYASC_H
