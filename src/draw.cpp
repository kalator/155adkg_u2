#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //put main direction line outside of visible canvas
    this->direction.setPoints(QPointF(-5.0,-5.0), QPointF(-5.0,-5.0));
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPen pen_poly(QColor(255, 0, 0, 127), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen_rect(QColor(0, 255, 0, 127), 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen_direction(QColor(0, 0, 255, 127), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen_point(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    //QPen pen_point_ch(Qt::red, 3);

    QPainter painter(this);

    //Draw points
    painter.setPen(pen_point);
    for(unsigned int i = 0; i < points.size(); i++)
    {
        painter.drawPoint(points[i]);
    }

    //Draw minimal rectangle
    painter.setPen(pen_rect);
    painter.drawPolygon(this->rect);

    //Draw convex hull
    painter.setPen(pen_poly);
    painter.drawPolygon(this->ch);

    //Draw main direction
    painter.setPen(pen_direction);
    painter.drawLine(this->direction);


}

void Draw::mousePressEvent(QMouseEvent *e)
{
    QPointF clicked_point(e->x(), e->y());
    points.push_back(clicked_point);
    repaint();
}

double Draw::setCH(std::string &selected_algorithm)
{   
    const double EPS = 10e-6;

    if(this->points.size() < 3)
        return 0;

    //check if there are at least 3 points with different coordinates
    int count = 0;
    QPointF p = points[0];
    for(unsigned int i = 0; i < this->points.size(); i++)
    {
        if((fabs(p.x() - points[i].x()) > EPS) && (fabs(p.y() - points[i].y()) > EPS))
        {
            p = points[i];
            count++;
        }
        if(count == 2)
            break;
    }
    if(count < 2)
    {
        return 0;
    }

    this->rect.clear();
    this->ch.clear();
    this->direction.setPoints(QPointF(-5.0,-5.0), QPointF(-5.0,-5.0));

    //start time
    clock_t s = std::clock();

    //choose algorithm
    if(selected_algorithm == "Jarvis Scan")
        this->ch = Algorithms::jarvisScanCH(this->points);
    else if(selected_algorithm == "Graham Scan")
        this->ch = Algorithms::grahamScanCH(this->points);
    else if(selected_algorithm == "Quick Hull")
        this->ch = Algorithms::quickHullCH(this->points);

    //end time
    clock_t e = std::clock();
    double time = (double) (e - s) / CLOCKS_PER_SEC;

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
    return time;
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
    this->points.reserve(count);
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
