#include "mylabel.h"
#include<QMouseEvent>
mylabel::mylabel(QWidget *parent ):QLabel(parent)
{

}
  void mylabel::mousePressEvent(QMouseEvent *ev)
  {
      //如果单击了就触发clicked信号



      //将该事件传给父类处理

      QLabel::mousePressEvent(ev);
      if (ev->button() == Qt::LeftButton)
      {

          //触发clicked信号

            emit label_clicked(true);

      }
  }
