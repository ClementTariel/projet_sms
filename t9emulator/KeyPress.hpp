
#include <QWidget>
#include <QLabel>
#include <QtGui>


#ifndef KEYPRESS_HPP
#define KEYPRESS_HPP


class KeyPress : public QWidget
{
    Q_OBJECT

public:
    KeyPress(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};


#endif // KEYPRESS_HPP
