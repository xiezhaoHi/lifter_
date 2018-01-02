#ifndef MYLABEL_H
#define MYLABEL_H
#include<QLabel>

class mylabel:public QLabel
{
    Q_OBJECT
public:
    explicit mylabel(QWidget *parent = 0);
   void mousePressEvent(QMouseEvent *ev);

Q_SIGNALS:
    void label_clicked(bool flag);
};

#endif // MYLABEL_H
