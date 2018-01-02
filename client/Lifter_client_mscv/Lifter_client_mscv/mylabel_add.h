#ifndef MYLABEL_ADD_H
#define MYLABEL_ADD_H

#include<QLabel>

class mylabel_add:public QLabel
{
    Q_OBJECT
public:
    explicit mylabel_add(QWidget *parent = 0);
   void mousePressEvent(QMouseEvent *ev);

Q_SIGNALS:
    void label_clicked(bool flag);
};
#endif // MYLABEL_ADD_H
