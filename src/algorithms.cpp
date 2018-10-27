#include "algorithms.h"

Algorithms::Algorithms()
{

}

TPositon Algorithms::getPointLinePosition(QPointF &q, QPointF &a, QPointF &b)
{
    double eps = 1.0e-10;

    //get vectors
    double ux = b.x() - a.x();
    double uy = b.y() - a.y();
    double vx = q.x() - a.x();
    double vy = q.y() - a.y();

    //get cross product
    double t = ux*vy - uy*vx;

    //point on the line - return -1
    if(fabs(t) < eps)
        return ON;

    //point in the left halfplane
    else if(t < 0)
        return LEFT;

    //point in the right halfplane
    else
        return RIGHT;
}

double Algorithms::getTwoVectorsAngle(QPointF &p1, QPointF &p2, QPointF &p3, QPointF &p4)
{
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    double dot = ux*vx + uy*vy;

    double nu = sqrt(ux*ux + uy*uy);
    double nv = sqrt(vx*vx + vy*vy);

    return acos(dot/(nu*nv))*180/M_PI;
}

QPolygonF Algorithms::jarvisScanCH(std::vector<QPointF> &points)
{
    const double EPS = 10e-6;

    QPolygonF poly_ch;

    //find pivot q
    std::sort(points.begin(), points.end(), SortByYAsc());
    QPointF q = points[0]; //sorted

    std::sort(points.begin(), points.end(), SortByXAsc());
    QPointF s = points[0];

    //create pjj
    QPointF pjj(s.x(), q.y());
    QPointF pj = q;
    if(fabs(pj.x()-pjj.x()) < EPS)
    {
        pjj.setX(pjj.x() + 100);
    }

    //add pivot to convex hull
    poly_ch.push_back(q);

    //cyklus
    do
    {
        int i_max = -1;
        double fi_max = 0;
        double min_dist = std::numeric_limits<double>::max();

        //Find pi = arg max angle (pi, pj, pjj)
        for(unsigned int i = 0; i<points.size(); i++)
        {
            //get angle between 2 segments
            double fi = getTwoVectorsAngle(pj, pjj, pj, points[i]);

            //find maximum
            if(fi > fi_max)
            {
                i_max = i;
                fi_max = fi;
            }

            else if(fabs(fi-fi_max) < EPS)
            {
                double dist = getDistance(pj, points[i]);
                if(min_dist > dist)
                {
                    min_dist = dist;
                    i_max = i;
                    fi_max = fi;
                }
            }
        }

        //add the next point to convex hull
        poly_ch.push_back(points[i_max]);

        //assign convex hull vertices
        pjj = pj;
        pj = points[i_max];
    }
    while(!(pj == q));

    return poly_ch;
}

std::vector<QPointF> Algorithms::generatePoints(QSize &canvas_size, int point_count, std::string shape)
{
    //get canvas height/width
    int h = canvas_size.height();
    int w = canvas_size.width();

    //create vector of new points
    std::vector<QPointF> random_points;

    //Star-shape
    if(shape == "Star-shape")
    {
        //seed
        srand(time(NULL));
        while(point_count--)
        {
            double x_new = 10 + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(w-10 - 10)));
            double y_new = 10 + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(h-10 - 10)));
            random_points.push_back(QPointF(x_new, y_new));
        }
    }

    //Circle
    else if(shape == "Circle")
    {
        int perimeter;
        w > h ? perimeter = h/2 - 10 : perimeter = w/2 - 10;
        for(int i = 0; i < point_count; i++)
        {
            double x = w/2.0 + perimeter*sin(i*2*M_PI/point_count);
            double y = h/2.0 + perimeter*cos(i*2*M_PI/point_count);
            random_points.push_back(QPointF(x,y));
        }
    }

    //Ellipse
    else if(shape == "Ellipse")
    {
        for(int i = 0; i < point_count; i++)
        {
            double x = w/2.0 + (w-10)/2.0 * sin(i*2*M_PI/point_count);
            double y = h/2.0 + (h-10)/2.0 * cos(i*2*M_PI/point_count);
            random_points.push_back(QPointF(x,y));
        }
    }

    //Square
    else if(shape == "Square")
    {
        int side_length;
        w > h ? side_length = h-10 : side_length = w-10;

        //firstly add 4 corner points
        random_points.push_back(QPointF(w/2.0-side_length/2.0,h/2.0-side_length/2.0));
        random_points.push_back(QPointF(w/2.0-side_length/2.0,h/2.0+side_length/2.0));
        random_points.push_back(QPointF(w/2.0+side_length/2.0,h/2.0-side_length/2.0));
        random_points.push_back(QPointF(w/2.0+side_length/2.0,h/2.0+side_length/2.0));

        for(int i = 1; i < (point_count)/4; i++)
        {
            random_points.push_back(QPointF(w/2.0-side_length/2.0+i*side_length/(point_count/4), h/2.0-side_length/2.0));
            random_points.push_back(QPointF(w/2.0-side_length/2.0+i*side_length/(point_count/4), h/2.0+side_length/2.0));
            random_points.push_back(QPointF(w/2.0-side_length/2.0, h/2.0-side_length/2.0+i*side_length/(point_count/4)));
            random_points.push_back(QPointF(w/2.0+side_length/2.0, h/2.0-side_length/2.0+i*side_length/(point_count/4)));
        }
    }
    return random_points;
}

void Algorithms::minimalRectangle(QPolygonF &poly_ch, QPolygonF &minimal_rectangle, QLineF &direction, bool compute_dir_line)
{
    std::vector<QPointF> points;
    for(int i=0; i<poly_ch.size()-1; i++)
    {
        points.push_back(poly_ch[i]);
    }

    QPointF p1, p2, p0; //p1 - actual point, p2 - next point, p0 - previous point

    double x_min_rect, x_max_rect, y_min_rect, y_max_rect;
    double min_volume = std::numeric_limits<double>::max();
    double angle_min;
    int n = points.size();
    for(int i=0; i<n; i++)
    {
        p1 = points[i%n];
        p2 = points[(i+1)%n];
        p0.setX(p1.x()-100);
        p0.setY(p1.y());

        double angle = getTwoVectorsAngle(p1, p0, p1, p2);
        rotateByAngle(points, angle);

        std::vector<QPointF> points_sort = points; //we do not want to sort points

        std::sort(points_sort.begin(), points_sort.end(), SortByXAsc());
        double x_min = points_sort[0].x();
        double x_max = points_sort[n-1].x();

        std::sort(points_sort.begin(), points_sort.end(), SortByYAsc());
        double y_min = points_sort[0].y();
        double y_max = points_sort[n-1].y();

        double volume = (x_max-x_min)*(y_max-y_min);
        if(min_volume > volume)
        {
            x_max_rect = x_max;
            x_min_rect = x_min;
            y_max_rect = y_max;
            y_min_rect = y_min;
            min_volume = volume;
            angle_min = angle;
        }

        //rotate back
        rotateByAngle(points, -angle);
    }

    minimal_rectangle.push_back(QPointF(x_min_rect, y_min_rect));
    minimal_rectangle.push_back(QPointF(x_min_rect, y_max_rect));
    minimal_rectangle.push_back(QPointF(x_max_rect, y_max_rect));
    minimal_rectangle.push_back(QPointF(x_max_rect, y_min_rect));

    rotateByAngle(minimal_rectangle, -angle_min);

    if(compute_dir_line)
    {
        //get main direction
        if((x_max_rect-x_min_rect) < (y_max_rect-y_min_rect))
        {
            direction.setP1(QPointF(x_min_rect, 0.5*(y_max_rect+y_min_rect)));
            direction.setP2(QPointF(x_max_rect, 0.5*(y_max_rect+y_min_rect)));
        }
        else
        {
            direction.setP1(QPointF(0.5*(x_min_rect+x_max_rect), y_min_rect));
            direction.setP2(QPointF(0.5*(x_min_rect+x_max_rect), y_max_rect));
        }

        rotateByAngle(direction, -angle_min);
    }
}

void Algorithms::rotateByAngle(std::vector<QPointF> &points, double angle)
{
    angle *= (M_PI/180);
    for(unsigned int i = 0; i < points.size(); i++)
    {
        QPointF temp_point = points[i];
        points[i].setX(cos(angle) * temp_point.x() + sin(angle) * temp_point.y());
        points[i].setY(-sin(angle) * temp_point.x() + cos(angle) * temp_point.y());
    }
}

void Algorithms::rotateByAngle(QPolygonF &points, double angle)
{
    angle *= (M_PI/180);
    for(int i = 0; i < points.size(); i++)
    {
        QPointF temp_point = points[i];
        points[i].setX(cos(angle) * temp_point.x() + sin(angle) * temp_point.y());
        points[i].setY(-sin(angle) * temp_point.x() + cos(angle) * temp_point.y());
    }
}

void Algorithms::rotateByAngle(QLineF &points, double angle)
{
    angle *= (M_PI/180);

    QPointF p1 = points.p1();
    QPointF p2 = points.p2();

    points.setP1(QPointF(cos(angle) * p1.x() + sin(angle) * p1.y(), -sin(angle) * p1.x() + cos(angle) * p1.y()));
    points.setP2(QPointF(cos(angle) * p2.x() + sin(angle) * p2.y(), -sin(angle) * p2.x() + cos(angle) * p2.y()));
}

double Algorithms::getDistance(QPointF &a, QPointF &b)
{
    return sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()));
}
