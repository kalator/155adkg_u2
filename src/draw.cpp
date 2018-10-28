#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //put main direction line outside of visible canvas
    this->direction.setPoints(QPointF(-5.0,-5.0), QPointF(-5.0,-5.0));
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

void Draw::setCH(std::string &selected_algorithm)
{   
    if(this->points.size() < 3)
        return;

    //check if there are at least 3 points with different coordinates
    int count = 0;
    QPointF p = points[0];
    for(int i = 0; i < this->points.size(); i++)
    {
        if((fabs(p.x() - points[i].x()) > 10e-6) && (fabs(p.y() - points[i].y()) > 10e-6))
        {
            p = points[i];
            count++;
        }
        if(count == 2)
            break;
    }
    if(count < 2)
    {
        return;
    }

        this->rect.clear();
    this->ch.clear();
    this->direction.setPoints(QPointF(-5.0,-5.0), QPointF(-5.0,-5.0));

    //choose algorithm
    if(selected_algorithm == "Jarvis Scan")
        this->ch = Algorithms::jarvisScanCH(this->points);
    else if(selected_algorithm == "Graham Scan")
        this->ch = Algorithms::grahamScanCH(this->points);

    //strictly convex hull
    int n = this->ch.size();
    for(int i = 0; i<n; i++)
    {
        double nx = - this->ch[(i+1)%n].y() + this->ch[i%n].y();
        double ny = this->ch[(i+1)%n].x() - this->ch[i%n].x();

        double c = -nx*this->ch[i%n].x() - ny*this->ch[i%n].y();

        if(fabs(nx*this->ch[(i+2)%n].x()+ny*this->ch[(i+2)%n].y()+c) < 10e-6)
        {
            this->ch.erase(this->ch.begin()+(i%n+1));
            n--;
            i--;
        }
    }

    repaint();
}

void Draw::setRect(bool draw_dir_line)
{
    rect.clear();
    if(this->ch.isEmpty())
        return;

    this->direction.setPoints(QPointF(-5.0,-5.0), QPointF(-5.0,-5.0));

    //compute minimal rectangle and optionaly main direction line
    Algorithms::minimalRectangle(this->ch, this->rect, this->direction, draw_dir_line);
    repaint();
}

void Draw::setPoints(QSizeF &canvas_size, int count, std::string &shape)
{
    clearCanvas();
    this->points = Algorithms::generatePoints(canvas_size, count, shape);
    repaint();
}

void Draw::clearCanvas()
{
    this->ch.clear();
    this->rect.clear();
    this->points.clear();
    this->direction.setPoints(QPointF(-5.0,-5.0), QPointF(-5.0,-5.0));
    repaint();
}
