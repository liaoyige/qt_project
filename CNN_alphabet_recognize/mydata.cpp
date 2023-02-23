#include "mydata.h"

MyData::MyData()
{

}

double MyData::Calculate(int ord,int num)
{
    //ord是待匹配
    //num是标准型
    double sum = standerd_weight[num][Lmax*4];
    for(int i = 0 ; i < Lmax*4 ; i ++ )
    {
        sum += standerd_weight[num][i]*ans_weight[ord][i];
    }
    return sum;
}

void MyData::Update(int ord, int num, double delta)
{
    for(int i = 0 ; i < Lmax*4 ; i ++ )
    {
        standerd_weight[num][i] = standerd_weight[num][i] + study_rate * delta * ans_weight[ord][i];
    }
    standerd_weight[num][Lmax*4] = standerd_weight[num][Lmax*4] - study_rate * delta;
}

void MyData::Graychange_image()
{
    QImage* newImage = new QImage(image.width(),image.height(),QImage::Format_ARGB32);
    QColor color;
    int r,g,b;
    for(int i = 0 ; i < image.width() ; i ++ )
    {
        for(int j = 0 ; j < image.height() ; j ++ )
        {
            color = image.pixel(i,j);
            r = color.red();
            g = color.green();
            b = color.blue();
            int tmp_ave = (r+g+b)/3;//均值灰度化
            newImage->setPixelColor(i,j,QColor(tmp_ave,tmp_ave,tmp_ave));
        }
    }
    image = *newImage;
    delete newImage;
}

void MyData::Filter_image()
{

//特别注意，这里的处理十分重要，也就是滤波处理，但是我们可以使用注释掉的高斯滤波，也可以用我们学过的均值滤波
//经过验证，此处的源代码段高斯滤波同样能够达到识别的目的

    /*QImage* newImage = new QImage(image.width(),image.height(),QImage::Format_ARGB32);
    QColor color;
    int gauss[3][3]={{1,2,1,},{2,4,2,},{1,2,1,},};//高斯模板
    int sum = 0 ;
    for(int i = 0 ; i < 3 ; i ++ )//高斯权重和
        for(int j = 0 ; j < 3 ; j ++ ){
            sum+=gauss[i][j];
        }
    int r,g,b;
    for(int i = 1 ; i < image.width()-1 ; i ++ )
    {
        for(int j = 1 ; j < image.height()-1 ; j ++ )
        {
            r = g = b =0;
            for(int x = -1 ; x <= 1 ; x ++ )
                for(int y = -1 ; y <= 1 ; y ++ )
                {
                    color = image.pixel(i+x,j+y);
                    r += color.red()*gauss[x+1][y+1];
                    g += color.green()*gauss[x+1][y+1];
                    b += color.blue()*gauss[x+1][y+1];
                }
            r = qBound(0,r/sum,255);
            g = qBound(0,g/sum,255);
            b = qBound(0,b/sum,255);
            newImage->setPixelColor(i,j,QColor(r,g,b));

        }
    }
    image = *newImage;
    delete newImage;*/
    //---------------------------------------------------------------------------------
    QImage* newImage = new QImage(image.width(), image.height(), QImage :: Format_ARGB32);
    int kernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}; //设计了一个卷积核
    QColor oldcolor;
    for(int x = 1; x < image.height() - 1; x++)
        for(int y = 1; y < image.width() - 1; y++)
        {
            int sum = 0;
            for(int i = -1; i <= 1; i++)
                for(int j = -1; j <= 1; j++)
                {
                    oldcolor = image.pixel(i+x, j+y);
                    sum += oldcolor.red() * kernel[i+1][j+1];
                }
            sum = qBound(0, sum/9, 255);
            newImage -> setPixelColor(x, y, QColor(sum, sum, sum));
        }
    image = *newImage;
    delete newImage;
    //---------------------------------------------------------------------------------

}

void MyData::Bitchange_image(int hold)
{
    QImage* newImage = new QImage(image.width(),image.height(),QImage::Format_ARGB32);
    QColor color;
    int r,g,b;
    for(int i = 0 ; i < image.width() ; i ++ )
    {
        for(int j = 0 ; j < image.height() ; j ++ )
        {
            color = image.pixel(i,j);
            r = color.red();
            g = color.green();
            b = color.blue();
            if(r >= hold)//二值化转换
                newImage->setPixelColor(i,j,QColor(255,255,255));
            else
                newImage->setPixelColor(i,j,QColor(0,0,0));
        }
    }
    image = *newImage;
    delete newImage;
}

void MyData::Thin_image()
{
    int clc = 1;
    QImage* newImage = new QImage(image.width(),image.height(),QImage::Format_ARGB32);
    QColor color,color_tmp;
    int num,w,e,nw,n,ne,sw,s,se;
    bool next = true;

    for(int i = 0 ; i < image.width() ; i ++ )
        for(int j = 0 ; j < image.height() ; j ++ )
            newImage->setPixelColor(i,j,image.pixelColor(i,j));

    while(clc > 0){
        clc = 0;
        next = true;
        for(int i = 1 ; i < image.width()-1 ; i ++ )
        {
            for(int j = 1 ; j < image.height()-1 ; j ++ )
            {
                if(!next){next = true;continue;};
                color = newImage->pixelColor(i,j);
                if(color.red() == 0)
                {
                    color_tmp = newImage->pixelColor(i,j-1);
                    w = color_tmp.red();
                    color_tmp = newImage->pixelColor(i,j+1);
                    e = color_tmp.red();
                    if((w == 255) || (e == 255))
                    {
                        color_tmp = newImage->pixelColor(i-1,j-1);
                        nw = color_tmp.red();
                        color_tmp = newImage->pixelColor(i-1,j);
                        n  = color_tmp.red();
                        color_tmp = newImage->pixelColor(i-1,j+1);
                        ne = color_tmp.red();
                        color_tmp = newImage->pixelColor(i+1,j-1);
                        sw = color_tmp.red();
                        color_tmp = newImage->pixelColor(i+1,j);
                        s  = color_tmp.red();
                        color_tmp = newImage->pixelColor(i+1,j+1);
                        se = color_tmp.red();
                        num = nw/255+(n/255)*2+(ne/255)*4+(w/255)*8+(e/255)*16+(sw/255)*32+(s/255)*64+(se/255)*128;
                        if (table_erase[num] == 1)
                        {
                            newImage->setPixelColor(i,j,QColor(255,255,255));
                            clc++;
                            next = false;
                        }
                    }
                }
            }
        }
        next = true;
        for(int j = 1 ; j < image.height()-1 ; j ++ )
        {
            for(int i = 1 ; i < image.width()-1 ; i ++ )
            {
                if(!next) {next = true;continue;};
                color = newImage->pixelColor(i,j);
                if(color.red() == 0)
                {
                    color_tmp = newImage->pixelColor(i-1,j);
                    n = color_tmp.red();
                    color_tmp = newImage->pixelColor(i+1,j);
                    s = color_tmp.red();
                    if((n == 255) || (s == 255))
                    {
                        color_tmp = newImage->pixelColor(i-1,j-1);
                        nw = color_tmp.red();
                        color_tmp = newImage->pixelColor(i-1,j+1);
                        ne = color_tmp.red();
                        color_tmp = newImage->pixelColor(i,j-1);
                        w  = color_tmp.red();
                        color_tmp = newImage->pixelColor(i,j+1);
                        e  = color_tmp.red();
                        color_tmp = newImage->pixelColor(i+1,j-1);
                        sw = color_tmp.red();
                        color_tmp = newImage->pixelColor(i+1,j+1);
                        se = color_tmp.red();
                        num = nw/255+(n/255)*2+(ne/255)*4+(w/255)*8+(e/255)*16+(sw/255)*32+(s/255)*64+(se/255)*128;
                        if (table_erase[num]==1)
                        {
                            newImage->setPixelColor(i,j,QColor(255,255,255));
                            clc++;
                            next = false;
                        }
                    }
                }
            }
        }

    }
    image = *newImage;
    delete newImage;
}
