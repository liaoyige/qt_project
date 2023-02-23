#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSlider>
#include<QSpinBox>
#include<qDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label = new QLabel(this);
    greylabel = new QLabel(this);
    binlabel  = new QLabel(this);
    label->setGeometry(0,0,400,200);
    greylabel->setGeometry(400,0,400,200);
    binlabel->setGeometry(800,0,400,200);

    QSlider* slider = new QSlider(Qt::Horizontal,this);
    slider->move(190,520);
    slider->setRange(0,255);
    slider->setValue(128);

    hold = slider->value();

    connect(slider,SIGNAL(valueChanged(int)),
            ui->spinBox,SLOT(setValue(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),
            slider,SLOT(setValue(int)));
    connect(slider,SIGNAL(valueChanged(int)),
            this,SLOT(on_pushButton_3_clicked(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QImage *MainWindow::greyImage(QImage *img)
{
   newImage = new QImage(img->width(),
                         img->height(),
                         QImage::Format_ARGB32);
   QColor oldcolor;
   for(int i=0;i<img->width();i++)
   {
       for(int j=0;j<img->height();j++)
       {
            oldcolor = img->pixel(i,j);
            int r = oldcolor.red();
            int g = oldcolor.green();
            int b = oldcolor.blue();
            int avarage = (r+g+b)/3;
            newImage->setPixelColor(i,j,QColor(avarage,avarage,avarage));
       }
   }
   return newImage;
}

QImage *MainWindow::binImage(QImage *img)
{
    newImage = new QImage(img->width(),
                          img->height(),
                          img->Format_ARGB32);
    QColor oldcolor;
    for(int i=0;i<img->width();i++)
    {
        for(int j=0;j<img->height();j++)
        {
            oldcolor = img->pixel(i,j);
            int r = oldcolor.red();
            int g = oldcolor.green();
            int b = oldcolor.blue();
            int color = (r+g+b)/3;
            if(color <= (255/2))
                newImage->setPixelColor(i,j,QColor(0,0,0));
            else
                newImage->setPixelColor(i,j,QColor(255,255,255));
        }
    }
    return newImage;
}


void MainWindow::on_pushButton_clicked()
{
//    pix.load(":/2.jpg");
    image.load(":/2.jpg");
    pix = pix.fromImage(image);
    label->setPixmap(pix);
}
//灰度图
void MainWindow::on_pushButton_2_clicked()
{
    newImage = greyImage(&image);
    pix = pix.fromImage(*newImage);
    greylabel->setPixmap(pix);
}

void MainWindow::on_pushButton_3_clicked()
{
    newImage = binImage(&image);
    pix = pix.fromImage(*newImage);
    binlabel->setPixmap(pix);
}



void MainWindow::on_spinBox_valueChanged(int arg1)
{
    this->hold = arg1  ;
    newImage = new QImage(image.width(),
                          image.height(),
                          image.Format_ARGB32);
    QColor oldcolor;
    for(int i=0;i<image.width();i++)
    {
        for(int j=0;j<image.height();j++)
        {
            oldcolor = image.pixel(i,j);
            int r = oldcolor.red();
            int g = oldcolor.green();
            int b = oldcolor.blue();
            int color = (r+g+b)/3;
            if(color <= arg1)
                newImage->setPixelColor(i,j,QColor(0,0,0));
            else
                newImage->setPixelColor(i,j,QColor(255,255,255));
        }
    }
    pix = pix.fromImage(*newImage);
    binlabel->setPixmap(pix);
}

