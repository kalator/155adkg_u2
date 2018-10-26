#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_createCHButton_clicked()
{
    if(ui->canvas->getPoints().size() < 3)
        return;
    std::vector<QPointF> points = ui->canvas->getPoints();
    QPolygonF ch = Algorithms::jarvisScanCH(points);
    ui->canvas->setCH(ch);
    repaint();
}

void Widget::on_generateButton_clicked()
{
    ui->canvas->clearCanvas();
    QSize canvas_size = ui->canvas->size();
    std::string shape = ui->shapeBox->currentText().toUtf8().constData();
    std::vector<QPointF> random_points = Algorithms::generatePoints(canvas_size, ui->pointCount->text().toInt(), shape);
    ui->canvas->setPoints(random_points);
    repaint();
}

void Widget::on_clearButton_clicked()
{
    ui->canvas->clearCanvas();
}

void Widget::on_createRectButton_clicked()
{
    QPolygonF poly_ch = ui->canvas->getPolygon();
    if(poly_ch.empty())
        return;
    ui->canvas->setRect();
    repaint();
}
