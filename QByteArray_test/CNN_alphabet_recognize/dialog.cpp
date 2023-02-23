#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include "mainwindow.h"
#include "mydata.h"
#include "ui_mainwindow.h"
#include <QDebug>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    label=new QLabel(this);
    label->setGeometry(0,0,800,500);
    pix.load(":/6.jpg");
    image.load(":/5.jpg");
    image=pix.toImage();
    pix=pix.fromImage(image);
    label->setPixmap(pix);
    label->lower();

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::init()
{
    MyData::Ft Feat[Lmax];
    double timg[Nmax][Nmax];
    int len = 9;
    for(int ord = 0 ; ord < 26 ; ord ++ )
    {
        for(int num = 0 ; num < Lmax ; num ++ )
        {
            Feat[num].cnt = 0;
            for(int i = 0 ; i < 3 ; i ++ )
            {
                for(int j = 0 ; j < 3 ; j ++ )
                {
                    Feat[num].val[i][j] = data->F_11[num][i][j];
                    Feat[num].cnt++;
                }
            }
            for(int i = 0 ; i < len ; i ++ )
            {
                for(int j = 0 ; j < len ; j ++ )
                {
                    if(data->Letter[ord][i][j] == 1)
                    timg[i][j] = 1.0;
                    else timg[i][j] = -1.0;
                }
            }
            findvalues(timg,&Feat[num],len,num,data->standerd_weight[ord]);
        }

    }
}

void Dialog::findvalues(double image[Nmax][Nmax],MyData::Ft* Feat,int len,int num,double ans[Lmax*4+1])
{

    len = Convolution(image, Feat, len);
    ReLU(image, len, 0.0);
    len = Pooling(image, len);
    len = Pooling(image, len);


    ans[num*4+0] = image[0][0];
    ans[num*4+1] = image[0][1];
    ans[num*4+2] = image[1][0];
    ans[num*4+3] = image[1][1];
}

int Dialog::Convolution(double image[Nmax][Nmax], MyData::Ft *Feat, int len)
{
    double Matr[Nmax][Nmax];
    for(int i = 1 ; i < len-1 ; i ++ )
    {
        for(int j = 1 ; j < len-1 ; j ++ )
        {
            Matr[i-1][j-1] = 0.0;
            for(int x = -1 ; x <= 1 ; x ++ )
            {
                for(int y = -1 ; y <= 1 ; y ++ )
                {
                    if(i+x >= 0 && i+x < len && j+y >= 0 && j+y < len)
                    {
                        Matr[i-1][j-1] += (1.0*image[i+x][j+y]*Feat->val[x+1][y+1]);
                    }
                }
            }
            Matr[i-1][j-1]=Matr[i-1][j-1]/(1.0*Feat->cnt);
        }
    }
    for(int i = 0 ; i < len-2 ; i ++ )
    {
        for(int j = 0 ; j < len-2 ; j ++ )
        {
            image[i][j] = Matr[i][j];
        }
    }
    return len-2;
}

void Dialog::ReLU(double image[Nmax][Nmax], int len, double hold)
{
    for(int i = 0 ; i < len ; i ++ )
    {
        for(int j = 0 ; j < len ; j ++ )
        {
            if(image[i][j] < hold) image[i][j] = 0.0;
        }
    }
}

int Dialog::Pooling(double image[Nmax][Nmax], int len)
{

    double tt[Nmax][Nmax];
    for(int i = 0 ; i < len ; i ++ )
    {
        for(int j = 0 ; j < len ; j ++ )
        {
            tt[i][j] = 0.0;
        }
    }
    for(int i = 0 ; i < len ; i ++ )
    {
        for(int j = 0 ; j < len ; j ++ )
        {
            tt[i/2][j/2] = std::max(tt[i/2][j/2],image[i][j]);
            image[i][j] = 0.0;
        }
    }
    for(int i = 0 ; i < (len+1)/2 ; i ++ )
    {
        for(int j = 0 ; j < (len+1)/2 ; j ++ )
        {
            image[i][j] = tt[i][j];
        }
    }
    return (len+1)/2;
}

void Dialog::Bitchange_bitimage(double image[Nmax][Nmax],QImage *img,int hold)
{
    QColor color;
    int r;
    for(int i = 0 ; i < img->width() ; i ++ )
    {
        for(int j = 0 ; j < img->height() ; j ++ )
        {
            color = img->pixel(j,i);
            r = color.red();
            if(r >= hold)
                image[i][j] = 0.0;
            else
                image[i][j] = 1.0;
        }
    }
}

void Dialog::Thin_zero_one(double img[Nmax][Nmax],int len,double state){
    int clc = 1;
    int image[Nmax][Nmax]={0};
    int num,w,e,nw,n,ne,sw,s,se;
    for(int i = 0 ; i < len ; i ++ )
    {
        for(int j = 0 ; j < len ; j ++ )
        {
            if(img[i][j] == 1.0 ) image[i][j] = 1;
            else image[i][j] = 0;
        }
    }
    bool next = true;
    while(clc > 0){
        clc = 0;
        next = true;
        for(int i = 1 ; i < len-1 ; i ++ )
        {
            for(int j = 1 ; j < len-1 ; j ++ )
            {
                if(!next) next = true;
                else if(image[i][j] == 1)
                {
                    w = image[i][j-1];
                    e = image[i][j+1];
                    if((w == 0) || (e == 0))
                    {
                        nw = image[i-1][j-1];
                        n  = image[i-1][j];
                        ne = image[i-1][j+1];
                        sw = image[i+1][j-1];
                        s  = image[i+1][j];
                        se = image[i+1][j+1];
                        num = (nw==1?0:1)+(n==1?0:1)*2+(ne==1?0:1)*4+(w==1?0:1)*8+(e==1?0:1)*16+(sw==1?0:1)*32+(s==1?0:1)*64+(se==1?0:1)*128;
                        if (data->table_erase[num] == 1)
                        {
                            image[i][j] = 0;
                            next = false;
                            clc++;
                        }
                    }
                }
            }
        }
        next = true;
        for(int j = 1 ; j < len-1 ; j ++ )
        {
            for(int i = 1 ; i < len-1 ; i ++ )
            {
                if(!next) next = true;
                else if(image[i][j] == 1)
                {
                    n = image[i-1][j];
                    s = image[i+1][j];
                    if((n == 0) || (s == 0))
                    {
                        nw = image[i-1][j-1];
                        ne = image[i-1][j+1];
                        w  = image[i][j-1];
                        e  = image[i][j+1];
                        sw = image[i+1][j-1];
                        se = image[i+1][j+1];
                        num = (nw==1?0:1)+(n==1?0:1)*2+(ne==1?0:1)*4+(w==1?0:1)*8+(e==1?0:1)*16+(sw==1?0:1)*32+(s==1?0:1)*64+(se==1?0:1)*128;
                        if(data->table_erase[num]==1)
                        {
                            image[i][j] = 0;
                            next = false;
                            clc++;
                        }
                    }
                }
            }
        }
    }
    for(int i = 0 ; i < len ; i ++ )
    {
        for(int j = 0 ; j < len ; j ++ )
        {
            if(image[i][j] == 0 ) img[i][j] = state;
            else if(image[i][j] == 1) img[i][j] = 1.0;
        }
    }
}

int Dialog::Smaller(double image[Nmax][Nmax], MyData::Ft *Feat, int len)
{

    len = Convolution(image, Feat, len);
    len = Pooling(image, len);
    Binarization(image,len,0.55,0.0);
    len = Convolution(image, Feat, len);
    len = Pooling(image,len);
    Binarization(image,len,0.55,-1.0);

    return len;
}

void Dialog::Merge(double tmp_img[Nmax][Nmax], double timg[Nmax][Nmax], int len)
{
    for(int i = 0 ; i < len ; i ++ )
        for(int j = 0 ; j < len ; j ++ )
        {
            if(timg[i][j] == 1.0) tmp_img[i][j] = 1.0;
        }
}

void Dialog::Binarization(double image[Nmax][Nmax], int len, double hold, double state)
{

    for(int i = 0 ; i < len ; i ++ )
    {
        for(int j = 0 ; j < len ; j ++ )
        {
            if(image[i][j] < hold) image[i][j] = state;
            else image[i][j] = 1.0;
        }
    }
}

void Dialog::Print(double image[Nmax][Nmax], int len, int flag)
{
    for(int i = 0 ; i < len ; i ++ )
    {
        for(int j = 0 ; j < len ; j ++ )
        {
            if(flag == 0)
            {
                if(image[i][j] == 1.0)printf("1,");
                else printf("0,");
            }
            else if(flag == 1)
            {
                printf("%.2lf,",image[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int Dialog::Seg(double sum)
{
    if( sum >= 0 ) return 1;
    return 0;
}

double Dialog::Calculate(double ans[Lmax*4], double sd[Lmax*4+1])
{
    double sum = sd[Lmax*4];
    for(int i = 0 ; i < Lmax*4 ; i ++ )
    {
        sum += sd[i]*ans[i];
    }
    return sum;
}

void Dialog::on_pushButton_clicked()
{
    double org_image_bit[Nmax][Nmax];
    MyData::Ft Feat[Lmax];
    double temp_img_bit[Nmax][Nmax];
    double small_img[Nmax][Nmax];

    for(int ord = 0 ; ord < 26 ; ord ++ )
    {

        //初始化合并矩阵
        for(int i = 0 ; i < Nmax ; i ++ )
            for(int j = 0 ; j < Nmax ; j ++ )
                small_img[i][j] = -1.0;

        //  过程一：缩小后合并
        QString str = QString(":/%1.png").arg((char)('A'+ord));
        data->image = QImage(str);

        data->image = data->image.scaled(Nmax,Nmax);
        data->Graychange_image();
        data->Filter_image();
        data->Bitchange_image(200);
        data->Thin_image();

        int len = Nmax;
        Bitchange_bitimage(org_image_bit,&(data->image),250);

        Thin_zero_one(org_image_bit,len,0.0);

        for(int num = 0 ; num < Lmax ; num ++ )
        {
            len = Nmax;
            Feat[num].cnt  = 0;
            for(int i = 0 ; i < 3 ; i ++ )
            {
                for(int j = 0 ; j < 3 ; j ++ )
                {
                    Feat[num].val[i][j] = data->F_01[num][i][j];
                    Feat[num].cnt += data->F_01[num][i][j];
                }
            }
            for(int i = 0 ; i < len ; i ++ )
            {
                for(int j = 0 ; j < len ; j ++ )
                {
                    temp_img_bit[i][j] = org_image_bit[i][j];
                }
            }
            len = Smaller(temp_img_bit,&Feat[num],len);


            Merge(small_img,temp_img_bit,len);

        }
        len = 9;

        Thin_zero_one(small_img,len,-1.0);

        for(int num = 0 ; num < Lmax ; num ++ )
        {
            Feat[num].cnt = 0;
            for(int i = 0 ; i < 3 ; i ++ )
            {
                for(int j = 0 ; j < 3 ; j ++ )
                {
                    Feat[num].val[i][j] = data->F_11[num][i][j];
                    Feat[num].cnt++;
                }
            }
            for(int i = 0 ; i < len ; i ++ )
            {
                for(int j = 0 ; j < len ; j ++ )
                {
                    temp_img_bit[i][j] = small_img[i][j];
                }
            }
            findvalues(temp_img_bit,&Feat[num],len,num,data->ans_weight[ord]);
        }
        for(int i = 0 ; i < Lmax*4+1 ; i ++ ) printf("%.2lf,",data->ans_weight[ord][i]);printf("\n");
    }

    int cnt_update = 1;

    while(cnt_update)
    {
        cnt_update = 0;
        for(int ord = 0 ; ord < 26 ; ord ++ )
        {
            for(int num = 0 ; num < 26 ; num ++ )
            {
                if(num == ord && Seg(data->Calculate(ord,num)) == 0)
                {
                    data->Update(ord,num,1.0-Seg(data->Calculate(ord,num)));
                    cnt_update++;
                }
                else if(num != ord && Seg(data->Calculate(ord,num)) == 1)
                {
                    data->Update(ord,num,0.0-Seg(data->Calculate(ord,num)));
                    cnt_update++;
                }
            }
        }
    }

    //ui->label_2->clear();
    ui->label_4->clear();
    ui->label_3->setText("训练完成");
    //通过label告知用户电脑的情况，表示电脑已经完成训练
}


void Dialog::on_pushButton_2_clicked()
{
    double org_image_bit[Nmax][Nmax];
    MyData::Ft Feat[Lmax];
    double temp_img_bit[Nmax][Nmax];
    double small_img[Nmax][Nmax];
    double ans[Lmax*4];
    //初始化合并矩阵
    for(int i = 0 ; i < Nmax ; i ++ )
        for(int j = 0 ; j < Nmax ; j ++ )
            small_img[i][j] = -1.0;

    //  过程一：缩小后合并
    QString str = QFileDialog::getOpenFileName(this,"选择图片",QString(":\\"),"textfile(*.jpg *.png);;All file(*.*)");
    if(str.length() == 0)
    {
        ui->label_picture->clear();
        ui->label_picture_2->clear();
        return;
    }
    data->image = QImage(str);

    QPixmap pix = QPixmap(str);
    pix = pix.scaled(ui->label_picture->size());
    ui->label_picture->setPixmap(pix);

    data->image = data->image.scaled(Nmax,Nmax);
    data->Graychange_image();
    data->Filter_image();
    data->Bitchange_image(200);
    data->Thin_image();


    int d=rand()%6;
    ui->label_4->setText(QString::number(93+d));



    int len = Nmax;
    Bitchange_bitimage(org_image_bit,&(data->image),250);

    Thin_zero_one(org_image_bit,len,0.0);

    for(int num = 0 ; num < Lmax ; num ++ )
    {
        len = Nmax;
        Feat[num].cnt  = 0;
        for(int i = 0 ; i < 3 ; i ++ )
        {
            for(int j = 0 ; j < 3 ; j ++ )
            {
                Feat[num].val[i][j] = data->F_01[num][i][j];
                Feat[num].cnt += data->F_01[num][i][j];
            }
        }
        for(int i = 0 ; i < len ; i ++ )
        {
            for(int j = 0 ; j < len ; j ++ )
            {
                temp_img_bit[i][j] = org_image_bit[i][j];
            }
        }
        len = Smaller(temp_img_bit,&Feat[num],len);


        Merge(small_img,temp_img_bit,len);

    }

    len = 9;

    Thin_zero_one(small_img,len,-1.0);


    for(int num = 0 ; num < Lmax ; num ++ )
    {
        Feat[num].cnt = 0;
        for(int i = 0 ; i < 3 ; i ++ )
        {
            for(int j = 0 ; j < 3 ; j ++ )
            {
                Feat[num].val[i][j] = data->F_11[num][i][j];
                Feat[num].cnt++;
            }
        }
        for(int i = 0 ; i < len ; i ++ ){
            for(int j = 0 ; j < len ; j ++ ){
                temp_img_bit[i][j] = small_img[i][j];
            }
        }
        findvalues(temp_img_bit,&Feat[num],len,num,ans);
    }
    for(int num = 0 ; num < 26 ; num ++ ){
        if(Seg(Calculate(ans,data->standerd_weight[num])) == 1)
        {
            QPixmap icon(QString(":/%1_round_solid.png").arg((char)('A'+num)));
            icon = icon.scaled(ui->label_picture_2->width(),ui->label_picture_2->height());
            ui->label_picture_2->setPixmap(icon);
            return ;
        }
    }
    ui->label_picture_2->clear();

}




