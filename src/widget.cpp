#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <fstream>

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
    QSizeF canvas_size = ui->canvas->size();
    //std::string shape = ui->shapeBox->currentText().toUtf8().constData();

    ///////////////////////////////////////////////////////////////////////////
    double timeJarvisRand[100];
    double timeJarvisGrid[100];
    double timeJarvisCirc[100];

    double timeQuickRand[100];
    double timeQuickGrid[100];
    double timeQuickCirc[100];

    double timeGrahamRand[100];
    double timeGrahamGrid[100];
    double timeGrahamCirc[100];

   /* double timeSweepRand[100];
    double timeSweepGrid[100];
    double timeSweepCirc[100]; */


    //time loop
    //1 000; 10 000; 50 000; 100 000; 250 000; 400 000; 550 000; 700 000; 850 000; 1 000 000;
    //Random; Grid; Circle
    int nn = 10;
    int np[10] = {1000,10000,50000,100000,250000,400000,550000,700000,850000,1000000};
    std::string shapeR = "Random";
    std::string shapeG = "Grid";
    std::string shapeC = "Circle";
    std::string algJ = "Jarvis Scan";
    std::string algQ = "Quick Hull";
    std::string algG = "Graham Scan";
    std::string algS = "Sweep Line";
    for(int i = 0; i < nn; i++)
    {

        for(int j = 0; j<nn; j++)
        {
            ui->timer->setText(QString::number(i*10+j));
            //Random
            ui->canvas->setPoints(canvas_size, np[i], shapeR);
            timeJarvisRand[i*nn+j] = ui->canvas->setCH(algJ);

            ui->canvas->setPoints(canvas_size, np[i], shapeR);
            timeQuickRand[i*nn+j] = ui->canvas->setCH(algQ);

            ui->canvas->setPoints(canvas_size, np[i], shapeR);
            timeGrahamRand[i*nn+j] = ui->canvas->setCH(algG);
/*
            ui->canvas->setPoints(canvas_size, np[i], shapeR);
            timeSweepRand[i*nn+j] = ui->canvas->setCH(algS);
*/
            //Grid
            ui->canvas->setPoints(canvas_size, np[i], shapeG);
            timeJarvisGrid[i*nn+j] = ui->canvas->setCH(algJ);

            ui->canvas->setPoints(canvas_size, np[i], shapeG);
            timeQuickGrid[i*nn+j] = ui->canvas->setCH(algQ);

            ui->canvas->setPoints(canvas_size, np[i], shapeG);
            timeGrahamGrid[i*nn+j] = ui->canvas->setCH(algG);
/*
            ui->canvas->setPoints(canvas_size, np[i], shapeG);
            timeSweepGrid[i*nn+j] = ui->canvas->setCH(algS);
*/
            //Circle
            ui->canvas->setPoints(canvas_size, np[i], shapeC);
            timeJarvisCirc[i*nn+j] = ui->canvas->setCH(algJ);

            ui->canvas->setPoints(canvas_size, np[i], shapeC);
            timeQuickCirc[i*nn+j] = ui->canvas->setCH(algQ);

            ui->canvas->setPoints(canvas_size, np[i], shapeC);
            timeGrahamCirc[i*nn+j] = ui->canvas->setCH(algG);
/*
            ui->canvas->setPoints(canvas_size, np[i], shapeC);
            timeSweepCirc[i*nn+j] = ui->canvas->setCH(algS);
*/
        }
    }

    std::ofstream file;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!TADY SE MENI CESTA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    file.open("/home/michael/skola/bayer/155adkg_u2/times.txt");


    file << "Times" << std::endl<<"----------------------------------" << std::endl;
    file << "1000,10000,50000,100000,250000,400000,550000,700000,850000,1000000" << std::endl;

    file  << std::endl << std::endl<< "Jarvis Scan" << std::endl;

    file <<"Random" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeJarvisRand[i*nn+j] << ",";
        }

        file  << std::endl;
    }

    file <<"Grid" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeJarvisGrid[i*nn+j] << ",";
        }

        file  << std::endl;
    }

    file <<"Circle" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeJarvisGrid[i*nn+j] << ",";
        }

        file << std::endl;
    }

    file  << std::endl << std::endl<< "Quick Hull" << std::endl;

    file <<"Random" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeQuickRand[i*nn+j] << ",";
        }

        file  << std::endl;
    }

    file <<"Grid" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeQuickGrid[i*nn+j] << ",";
        }

        file  << std::endl;
    }

    file <<"Circle" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeQuickGrid[i*nn+j] << ",";
        }

        file  << std::endl;
    }

    file  << std::endl << std::endl<< "Graham Scan" << std::endl;

    file <<"Random" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeGrahamRand[i*nn+j] << ",";
        }

        file  << std::endl;
    }

    file <<"Grid" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeGrahamGrid[i*nn+j] << ",";
        }

        file  << std::endl;
    }

    file <<"Circle" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeGrahamGrid[i*nn+j] << ",";
        }

        file  << std::endl;
    }
    /*
    file  << std::endl << std::endl<< "Swwep Line" << std::endl;

    file <<"Random" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeSweepRand[i*nn+j] << ",";
        }

        file  << std::endl;
    }

    file <<"Grid" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeSweepGrid[i*nn+j] << ",";
        }

        file  << std::endl;
    }

    file <<"Circle" << std::endl;

    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            file << timeSweepGrid[i*nn+j] << ",";
        }

        file  << std::endl;
    }
    */




    file.close();
////////////////////////////////////////////////////////////////////////////////////



    //generate points
    //ui->canvas->setPoints(canvas_size, ui->pointCount->text().toInt(), shape);
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
