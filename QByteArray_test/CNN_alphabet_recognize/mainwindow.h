#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mydata.h"

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QMainWindow>
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QTimer>
#include "dialog.h"

#define Nmax 42
#define Lmax 6

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Dialog mydialog;

    MyData* data = new MyData();

private slots:
    //void on_pushButton_clicked();

   // void on_pushButton_compare_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void change_background_color();
private:
    Ui::MainWindow *ui;


    QMediaPlayer* player;
    QVideoWidget* videoWidget;
    QMediaPlaylist* playlist;
    QTimer* mytimer;


//    void init();

//    void findvalues(double image[Nmax][Nmax],MyData::Ft* Feat1,int len,int num,double ans[Lmax*4+1]);

//    int Convolution(double image[Nmax][Nmax], MyData::Ft *Feat, int len);

//    void ReLU(double image[Nmax][Nmax], int len, double hold);

//    int Pooling(double image[Nmax][Nmax], int len);

//    void Bitchange_bitimage(double image[Nmax][Nmax],QImage *img,int hold);

//    void Thin_zero_one(double img[Nmax][Nmax],int len,double state = 0.0);

//    int Smaller(double image[Nmax][Nmax], MyData::Ft *Feat, int len);

//    void Merge(double tmp_img[Nmax][Nmax], double timg[Nmax][Nmax], int len);

//    void Binarization(double image[Nmax][Nmax], int len, double hold, double state = 0.0);

//    void Print(double image[Nmax][Nmax], int len,int flag);

//    int Seg(double sum);

//    double Calculate(double ans[Lmax*4],double sd[Lmax*4+1]);



};

#endif // MAINWINDOW_H
