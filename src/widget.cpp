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
    //check if there are at least 3 points
    if(ui->canvas->getPoints().size() < 3)
        return;
    std::vector<QPointF> points = ui->canvas->getPoints();\

    QPolygonF ch;
    //choose algorithm
    if(ui->methodCombo->currentText() == "Jarvis Scan")
        ch = Algorithms::jarvisScanCH(points);
    else if(ui->methodCombo->currentText() == "Graham Scan")
        ch = Algorithms::grahamScanCH(points);
    ui->canvas->setCH(ch);
    repaint();
}

void Widget::on_generateButton_clicked()
{
    ui->canvas->clearCanvas();
    QSize canvas_size = ui->canvas->size();
    std::string shape = ui->shapeBox->currentText().toUtf8().constData();

    //generate points
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

    //check if there is any convex hull computed
    if(poly_ch.empty())
        return;

    //set minimal bounding rectangle and if checked, also main direction line
    ui->canvas->setRect(ui->checkBox->isChecked());
    repaint();
}

void Widget::on_helpButton_clicked()
{
    //show help
    HelpDialog help_dialog;
    help_dialog.setModal(true);
    help_dialog.exec();
}
