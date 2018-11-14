#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <fstream>
#include <QDebug>

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
    ui->timer->clear();

    std::string selected_algorithm = ui->methodCombo->currentText().toUtf8().constData();
    double time = ui->canvas->setCH(selected_algorithm);

    //write time
    ui->timer->setText(QString::number(time, 'f', 3) + " s");
}

void Widget::on_generateButton_clicked()
{
    QSize canvas_size = ui->canvas->size();
    std::string shape = ui->shapeBox->currentText().toUtf8().constData();

    //generate points
    ui->canvas->setPoints(canvas_size, ui->pointCount->text().toInt(), shape);
}

void Widget::on_clearButton_clicked()
{
    ui->timer->clear();
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

void Widget::on_timeStatButton_clicked()
{
    QSize canvas_size = ui->canvas->size();

    std::string shape = ui->shapeBox->currentText().toUtf8().constData();

    std::string selected_algorithm = ui->methodCombo->currentText().toUtf8().constData();

    double stat[100];

    //time loop
    int np[10] = {1000,10000,100000,230000,360000,490000,620000,750000,880000,1000000};
    int nn = 10;
    for(int i = 0; i < nn; i++)
    {

        ui->canvas->setPoints(canvas_size, np[i], shape);


        for(int j = 0; j<nn; j++)
        {
            ui->timer->setText(QString::number(i*10+j));

            stat[i*nn+j] = ui->canvas->setCH(selected_algorithm);
            qDebug() <<np[i] << " points: " << stat[i*nn+j];
        }
    }

    std::ofstream file;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!TADY SE MENI CESTA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    std::string file_path = "/home/michael/skola/bayer/155adkg_u2/"; //Tady, do slozky



    file.open(file_path + "times_" + selected_algorithm+"_"+shape+".txt");

    file << "1000,10000,100000,230000,360000,490000,620000,750000,880000,1000000" << std::endl;
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j<91; j +=10)
        {
            if(j<90)
            {
                file << stat[i+j] << ",";
            }
            else
                file << stat[i+j];
        }
        file << std::endl;
    }


    file.close();
}
