#include <QCoreApplication>
#include <QByteArray>
#include <QByteArrayMatcher>
#include <QByteArrayList>
#include <QDebug>

/*
     QByteArray  与   QString 区别联系
   1、QByteArray 和 QString都是存储字节的容器，其中包括字符串
   2、QString类主要存储16位的unicode字节，更加方便存储像汉字这样的无ASCLL对应的字符串。
   3、QByteArray类主要存储原始的二进制数据，运用在嵌入式上更多一些。
*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QByteArray array("am ");
    array.append("tired ");
    array.prepend("I ");
    qDebug() << "Origin byte array: " << array;

    QByteArrayMatcher matcher("tired");
    int pos = matcher.indexIn(array);
    if(pos == -1)
    {
        qDebug() << "Search failed";
    }
    else
    {
        qDebug() << "The position is " << pos;
    }

    QByteArrayList byteList;
    byteList << "I" << "like" << "Qt";
    qDebug() << "byteList: " << byteList;
    QByteArray joinArray = byteList.join("-");
    qDebug() << "joinArray: " << joinArray;

    return a.exec();
}
