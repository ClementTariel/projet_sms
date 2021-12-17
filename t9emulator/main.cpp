#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QPushButton>
#include <QRect>
#include <QScreen>

#include <QtGui>
#include "KeyPress.hpp"
#include "common.hpp"

char global_buffer[64];
int global_buffer_pos;

int main(int argc, char *argv[])
{
    global_buffer_pos = 0;


    QApplication app(argc, argv);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int physical_height = screenGeometry.height();
    int physical_width = screenGeometry.width();

    QWidget *widget = new QWidget;
    QLabel  *label  = new QLabel;
    QPixmap *pixmap_img = new QPixmap(QCoreApplication::applicationDirPath()+"/1110i.jpg");
    // mon_logo se trouve dans le repertoire qui contient mon exe
    //int w = label->width();
    //int h = label->height();
    //taille prise au pif
    int w = (int)(physical_width*0.9);
    int h = (int)(physical_height*0.9);

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    label->setPixmap(pixmap_img->scaled(w,h,Qt::KeepAspectRatio));
    //label->setPixmap(*pixmap_img);

    QGridLayout *gridLayout = new QGridLayout(widget);
    gridLayout->addWidget(label, 0, 0);
    widget->setLayout(gridLayout);

    int xanchor1 = (int)(h*0.052);
    int yanchor1 = (int)(h*0.683);
    int xsize1 = (int)(h*0.12);
    int ysize1 = (int)(h*0.07);
    int deltax = (int)(h*0.122);
    int deltay = (int)(h*0.073);

    QPushButton *button1 = new QPushButton(widget);
    button1->setText("1");
    button1->move(xanchor1, yanchor1);
    button1->setFixedSize(xsize1,ysize1);

    QPushButton *button2 = new QPushButton(widget);
    button2->setText("2");
    button2->move(xanchor1+deltax, yanchor1);
    button2->setFixedSize(xsize1,ysize1);

    QPushButton *button3 = new QPushButton(widget);
    button3->setText("3");
    button3->move(xanchor1+2*deltax, yanchor1);
    button3->setFixedSize(xsize1,ysize1);

    QPushButton *button4 = new QPushButton(widget);
    button4->setText("4");
    button4->move(xanchor1, yanchor1+deltay);
    button4->setFixedSize(xsize1,ysize1);

    QPushButton *button5 = new QPushButton(widget);
    button5->setText("5");
    button5->move(xanchor1+deltax, yanchor1+deltay);
    button5->setFixedSize(xsize1,ysize1);

    QPushButton *button6 = new QPushButton(widget);
    button6->setText("6");
    button6->move(xanchor1+2*deltax, yanchor1+deltay);
    button6->setFixedSize(xsize1,ysize1);

    QPushButton *button7 = new QPushButton(widget);
    button7->setText("7");
    button7->move(xanchor1, yanchor1+2*deltay);
    button7->setFixedSize(xsize1,ysize1);

    QPushButton *button8 = new QPushButton(widget);
    button8->setText("8");
    button8->move(xanchor1+deltax, yanchor1+2*deltay);
    button8->setFixedSize(xsize1,ysize1);

    QPushButton *button9 = new QPushButton(widget);
    button9->setText("9");
    button9->move(xanchor1+2*deltax, yanchor1+2*deltay);
    button9->setFixedSize(xsize1,ysize1);

    QPushButton *button_etoile = new QPushButton(widget);
    button_etoile->setText("*");
    button_etoile->move(xanchor1, yanchor1+3*deltay);
    button_etoile->setFixedSize(xsize1,ysize1);

    QPushButton *button0 = new QPushButton(widget);
    button0->setText("0");
    button0->move(xanchor1+deltax, yanchor1+3*deltay);
    button0->setFixedSize(xsize1,ysize1);

    QPushButton *button_diese = new QPushButton(widget);
    button_diese->setText("#");
    button_diese->move(xanchor1+2*deltax, yanchor1+3*deltay);
    button_diese->setFixedSize(xsize1,ysize1);



    KeyPress *keyPress = new KeyPress(widget);

    //KeyPress *keyPress = new KeyPress();
    //keyPress->show();


    widget->show();


    return app.exec();
}
