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
    //poly_ch.push_back(q);

    //loop
    do
    {
        int i_max = -1;
        double fi_max = 0;
        double min_dist = std::numeric_limits<double>::max(); //set min_dist for colinear poins

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

            //in case of colinear points, get one, that is closest to point pj
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

QPolygonF Algorithms::grahamScanCH(std::vector<QPointF> &points)
{
    QPolygonF poly;

    const double EPS = 10e-6;

    std::sort(points.begin(), points.end(), SortByXAsc());
    QPointF s = points[0];

    //find pivot q
    std::sort(points.begin(), points.end(), SortByYAsc());
    QPointF q = points[0]; //sorted

    //point that together with point q creates a parallel with x-axis
    s.setY(q.y());

    if(fabs(q.x()-s.x())< EPS)
    {
        s.setX(s.x()+100);
    }

    //sort by angle (and by distance from q if angle is the same), leave q on the first place
    points.erase(points.begin());
    std::sort(points.begin(), points.end(), SortByAngleAsc(q));
    points.insert(points.begin(),q);

    //go through sorted points and rule out those that have same angle as some other and are closer to q
    //save cleared points to vector
    //unreduce coordinates
    std::vector<QPointF> points_cleared;
    points_cleared.push_back(q);

    for(unsigned int i = 1; i < points.size()-1; i++)
    {
        if(fabs(getTwoVectorsAngle(q, s, q, points[i])-getTwoVectorsAngle(q, s, q, points[i+1])) > 0)
        {
            points_cleared.push_back(points[i]);
        }
        if(i == points.size()-2)
        {
            points_cleared.push_back(points[i+1]);
        }
    }

    QPolygonF poly_ch;
    poly_ch.push_back(points_cleared[0]);
    poly_ch.push_back(points_cleared[1]);

    for(unsigned int i = 2; i < points_cleared.size(); i++)
    {
        bool notConvex = true;
        while(notConvex)
        {
            if(getPointLinePosition(poly_ch[poly_ch.size()-1], poly_ch[poly_ch.size()-2], points_cleared[i]) == LEFT)
                    poly_ch.pop_back();
            else
                notConvex = false;
        }
        poly_ch.push_back(points_cleared[i]);
    }

    return poly_ch;
}


std::vector<QPointF> Algorithms::generatePoints(QSizeF &canvas_size, int point_count, std::string shape)
{
    //SS stands for side_strip - thickness of space on sides, that should be empty (to avoid point/polygon drawing, that is not visible)
    const int SS = 10;

    //get canvas height/width
    int h = canvas_size.height();
    int w = canvas_size.width();

    //create vector of new points
    std::vector<QPointF> random_points;

    //Star-shape
    if(shape == "Star-shape")
    {
        //generate random points
        srand(time(NULL));
        while(point_count--)
        {
            double x_new = SS + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(w-SS - SS)));
            double y_new = SS + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(h-SS - SS)));
            random_points.push_back(QPointF(x_new, y_new));
        }
    }

    //Circle
    else if(shape == "Circle")
    {
        int perimeter;
        w > h ? perimeter = h/2 - SS : perimeter = w/2 - SS;
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
            double x = w/2.0 + (w-SS)/2.0 * sin(i*2*M_PI/point_count);
            double y = h/2.0 + (h-SS)/2.0 * cos(i*2*M_PI/point_count);
            random_points.push_back(QPointF(x,y));
        }
    }

    //Square
    else if(shape == "Square")
    {
        int side_length;
        w > h ? side_length = h-SS : side_length = w-SS;

        //firstly add 4 corner points
        random_points.push_back(QPointF(w/2.0-side_length/2.0,h/2.0-side_length/2.0));
        random_points.push_back(QPointF(w/2.0-side_length/2.0,h/2.0+side_length/2.0));
        random_points.push_back(QPointF(w/2.0+side_length/2.0,h/2.0-side_length/2.0));
        random_points.push_back(QPointF(w/2.0+side_length/2.0,h/2.0+side_length/2.0));

        //add rest of points
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
    //vector of points to copy poly_ch points into, so poly_ch will not be affected by following operations
    std::vector<QPointF> points;
    for(int i=0; i<poly_ch.size(); i++)
    {
        points.push_back(poly_ch[i]);
    }

    //compute the minimal bounding rectangle
    QPointF p1, p2, p0; //p1 - actual point, p2 - next point, p0 - previous point
    double x_min_rect, x_max_rect, y_min_rect, y_max_rect; //coordinates of minimal bounding rectangle corners
    double min_volume = std::numeric_limits<double>::max();
    double angle_min;
    int n = points.size();
    for(int i=0; i<n; i++)
    {
        p1 = points[i%n];
        p2 = points[(i+1)%n];
        p0.setX(p1.x()-1000); //set a parallel to x-axis long enough for angle computing
        p0.setY(p1.y());

        //compute angle to rotate with
        double angle = getTwoVectorsAngle(p1, p0, p1, p2);
        rotateByAngle(points, angle);
        qDebug() << "angle " << angle;

        //copy points into new container for sorting in order to keep original sequence of poly points
        std::vector<QPointF> points_sort = points;

        //find coordinates of bounding rectangle
        std::sort(points_sort.begin(), points_sort.end(), SortByXAsc());
        double x_min = points_sort[0].x();
        double x_max = points_sort[n-1].x();
        std::sort(points_sort.begin(), points_sort.end(), SortByYAsc());
        double y_min = points_sort[0].y();
        double y_max = points_sort[n-1].y();

        //compute volume of bounding rectangle
        double volume = (x_max-x_min)*(y_max-y_min);

        //get minimal-area bounding rectangle
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

    //add minimal bounding rectangle points
    minimal_rectangle.push_back(QPointF(x_min_rect, y_min_rect));
    minimal_rectangle.push_back(QPointF(x_min_rect, y_max_rect));
    minimal_rectangle.push_back(QPointF(x_max_rect, y_max_rect));
    minimal_rectangle.push_back(QPointF(x_max_rect, y_min_rect));

    //rotate minimal bounding rectangle back (coordinates were rotated from loop)
    rotateByAngle(minimal_rectangle, -angle_min);

    //compute main direction line
    if(compute_dir_line)
    {
        //decide what minimal boundaring rectangle side is longer and set line
        if((x_max_rect-x_min_rect) > (y_max_rect-y_min_rect))
        {
            direction.setP1(QPointF(x_min_rect, 0.5*(y_max_rect+y_min_rect)));
            direction.setP2(QPointF(x_max_rect, 0.5*(y_max_rect+y_min_rect)));
        }
        else
        {
            direction.setP1(QPointF(0.5*(x_min_rect+x_max_rect), y_min_rect));
            direction.setP2(QPointF(0.5*(x_min_rect+x_max_rect), y_max_rect));
        }

        //rotate main direction line back (coordinates were rotated from loop)
        rotateByAngle(direction, -angle_min);
    }
    qDebug() << "Minimal rectangle " << minimal_rectangle[0] << " " << minimal_rectangle[1] << " " << minimal_rectangle[2] << " " << minimal_rectangle[3];
    qDebug() << "Volume: " << min_volume;
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
