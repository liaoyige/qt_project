#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mydata.h"
#include <QLabel>
#define Nmax 42
#define Lmax 6

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QPixmap pix;
    QImage  image;

    MyData* data = new MyData();
    QLabel* label;

private slots:
    void on_pushButton_clicked();

    //void on_pushButton_compare_clicked();

    void on_pushButton_2_clicked();


private:
    Ui::Dialog *ui;

    void init();

    void findvalues(double image[Nmax][Nmax],MyData::Ft* Feat1,int len,int num,double ans[Lmax*4+1]);

    int Convolution(double image[Nmax][Nmax], MyData::Ft *Feat, int len);

    void ReLU(double image[Nmax][Nmax], int len, double hold);

    int Pooling(double image[Nmax][Nmax], int len);

    void Bitchange_bitimage(double image[Nmax][Nmax],QImage *img,int hold);

    void Thin_zero_one(double img[Nmax][Nmax],int len,double state = 0.0);

    int Smaller(double image[Nmax][Nmax], MyData::Ft *Feat, int len);

    void Merge(double tmp_img[Nmax][Nmax], double timg[Nmax][Nmax], int len);

    void Binarization(double image[Nmax][Nmax], int len, double hold, double state = 0.0);

    void Print(double image[Nmax][Nmax], int len,int flag);

    int Seg(double sum);

    double Calculate(double ans[Lmax*4],double sd[Lmax*4+1]);

};

#endif // DIALOG_H
