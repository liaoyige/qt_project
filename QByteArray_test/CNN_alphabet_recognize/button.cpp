#include"button.h"


void Button::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.fillRect(0,0,size().width(), size().height(), QColor(255,0,0,100));
}
