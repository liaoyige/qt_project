#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSlider>
#include<QSpinBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label = new QLabel(this);
    greylabel = new QLabel(this);
    binlabel  = new QLabel(this);
    averagelabel = new QLabel(this);
    shacklabel = new QLabel(this);
    Lowlabel = new QLabel(this);

    label->setGeometry(0,0,400,200);
    greylabel->setGeometry(400,0,400,200);
    binlabel->setGeometry(800,0,400,200);
    averagelabel->setGeometry(0,220,400,200);
    shacklabel->setGeometry(400,220,400,200);
    Lowlabel->setGeometry(800,220,400,200);

    QSlider* slider = new QSlider(Qt::Horizontal,this);
    slider->move(1030,590);
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
//    int kernel[3][3] = {{1,1,1},
//                        {1,1,1},
//                        {1,1,1} };
QImage *MainWindow::averageFilter(QImage *img)
{
    QImage* newImage = new QImage(img->width(),
                                 img->height(),
                                 QImage::Format_ARGB32);

    int gauss[3][3] = {{1,2,1},
                       {2,4,2},
                       {1,2,1} };
    QColor oldcolor;
    for(int x=1;x<img->width();x++)
    {
        for(int y=1;y<img->height();y++)
        {
            int sum = 0;
            for(int i=-1;i<=1;i++)
                for(int j=-1;j<=1;j++)
                {
                    oldcolor = img->pixel(i+x,j+y);
                    sum+=oldcolor.red()*gauss[i+1][j+1];
                }
            sum = qBound(0,sum/16,255);
            newImage->setPixelColor(x,y,QColor(sum,sum,sum));
        }
    }

    return newImage;
}

QImage *MainWindow::LowFilter(QImage *img)
{
    QImage* newImage = new QImage(img->width(),
                                 img->height(),
                                 QImage::Format_ARGB32);
    int kernel[3][3] = {{1,1,1},
                        {1,-7,1},
                        {1,1,1} };
    QColor oldcolor;
    for(int x=1;x<img->width();x++)
    {
        for(int y=1;y<img->height();y++)
        {
            int sum = 0;
            for(int i=-1;i<=1;i++)
                for(int j=-1;j<=1;j++)
                {
                    oldcolor = img->pixel(i+x,j+y);
                    sum+=oldcolor.red()*kernel[i+1][j+1];
                }
            sum = qBound(0,sum/1,255);
            newImage->setPixelColor(x,y,QColor(sum,sum,sum));
        }
    }
    return newImage;
}

QImage *MainWindow::ShackTreat(QImage *img)
{
    QImage* newImage = new QImage(img->width(),
                                 img->height(),
                                 QImage::Format_ARGB32);

    int shack[3][3] = {{-1,-1,-1},
                       {-1,9,-1},
                       {-1,-1,-1} };
    QColor oldcolor;
    for(int x=1;x<img->width();x++)
    {
        for(int y=1;y<img->height();y++)
        {
            int sum = 0;
            for(int i=-1;i<=1;i++)
                for(int j=-1;j<=1;j++)
                {
                    oldcolor = img->pixel(i+x,j+y);
                    sum+=oldcolor.red()*shack[i+1][j+1];
                }
            sum = qBound(0,sum/1,255);
            newImage->setPixelColor(x,y,QColor(sum,sum,sum));
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

    //均值滤波  降低噪声点，图像变模糊
void MainWindow::on_pushButton_4_clicked()
{
    image = *averageFilter(&image);
    pix = pix.fromImage(image);
    averagelabel->setPixmap(pix);
}

void MainWindow::on_pushButton_5_clicked()
{
    image = *ShackTreat(&image);
    pix = pix.fromImage(image);
    shacklabel->setPixmap(pix);
}

void MainWindow::on_pushButton_6_clicked()
{
    image = *LowFilter(&image);
    pix = pix.fromImage(image);
    Lowlabel->setPixmap(pix);
}
