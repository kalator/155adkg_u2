#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //put main direction line outside of visible canvas
    this->direction.setPoints(QPointF(-5,-5), QPointF(-5,-5));
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPen pen_poly(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen_rect(Qt::green, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen_direction(Qt::blue, 4);
    QPen pen_point(Qt::black, 1);
    //QPen pen_point_ch(Qt::red, 3);

    QPainter painter(this);

    //Draw minimal rectangle
    painter.setPen(pen_rect);
    painter.drawPolygon(this->rect);

    //Draw convex hull
    painter.setPen(pen_poly);
    painter.drawPolygon(this->ch);

    //Draw main direction
    painter.setPen(pen_direction);
    painter.drawLine(this->direction);

    //Draw points
    painter.setPen(pen_point);
    for(unsigned int i = 0; i < points.size(); i++)
    {
        painter.drawEllipse(points[i].x()-2, points[i].y()-2,4,4);
    }
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    QPointF clicked_point(e->x(), e->y());
    points.push_back(clicked_point);
    repaint();
}

void Draw::setCH(QPolygonF ch)
{
    this->rect.clear();
    this->direction.setPoints(QPointF(-5,-5), QPointF(-5,-5));
    this->ch = ch;
}

void Draw::setRect(bool draw_dir_line)
{
    rect.clear();

    this->direction.setPoints(QPointF(-5,-5), QPointF(-5,-5));

    //compute minimal rectangle and optionaly main direction line
    Algorithms::minimalRectangle(this->ch, this->rect, this->direction, draw_dir_line);
}


void Draw::clearCanvas()
{
    this->ch.clear();
    this->rect.clear();
    this->points.clear();
    this->direction.setPoints(QPointF(-5,-5), QPointF(-5,-5));
    repaint();
}
