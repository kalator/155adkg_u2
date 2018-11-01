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
    //start time
    clock_t s = std::clock();

    std::string selected_algorithm = ui->methodCombo->currentText().toUtf8().constData();
    ui->canvas->setCH(selected_algorithm);

    //end time
    clock_t e = std::clock();
    clock_t time = float (e - s) / CLOCKS_PER_SEC;

    //write time
    ui->timer->setText(QString::number(time));
}

void Widget::on_generateButton_clicked()
{
    QSizeF canvas_size = ui->canvas->size();
    std::string shape = ui->shapeBox->currentText().toUtf8().constData();
    //generate points
    ui->canvas->setPoints(canvas_size, ui->pointCount->text().toInt(), shape);
}

void Widget::on_clearButton_clicked()
{
    ui->canvas->clearCanvas();
}

void Widget::on_createRectButton_clicked()
{
    //set minimal bounding rectangle and if checked, also main direction line
    ui->canvas->setRect(ui->checkBox->isChecked());
}

void Widget::on_helpButton_clicked()
{
    //show help
    HelpDialog help_dialog;
    help_dialog.setModal(true);
    help_dialog.exec();
}
