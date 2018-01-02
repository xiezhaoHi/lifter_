#include "mylabel_add.h"
#include"QMouseEvent"

mylabel_add::mylabel_add(QWidget *parent ):QLabel(parent)
{

}
void mylabel_add::mousePressEvent(QMouseEvent *ev)
{
    //如果单击了就触发clicked信号

    if (ev->button() == Qt::LeftButton)

    {

        //触发clicked信号

          emit label_clicked(true);

    }

    //将该事件传给父类处理

    QLabel::mousePressEvent(ev);
}
