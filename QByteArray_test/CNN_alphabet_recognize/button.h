#ifndef BUTTON_H
#define BUTTON_H

#include<QPushButton>
#include<QPainter>
class Button:public QPushButton
{
    Q_OBJECT
public:
    Button():QPushButton(0){}
    void paintEvent(QPaintEvent* e)override;
};

#endif // BUTTON_H
