#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QPushButton>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *widget = new QWidget;
    QLabel  *label  = new QLabel;
    QPixmap *pixmap_img = new QPixmap(QCoreApplication::applicationDirPath()+"/1110i.jpg");
    // mon_logo se trouve dans le repertoire qui contient mon exe
    //int w = label->width();
    //int h = label->height();
    //taille prise au pif
    int w = 650;
    int h = 650;

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    label->setPixmap(pixmap_img->scaled(w,h,Qt::KeepAspectRatio));
    //label->setPixmap(*pixmap_img);

    QGridLayout *gridLayout = new QGridLayout(widget);
    gridLayout->addWidget(label, 0, 0);
    widget->setLayout(gridLayout);



    QPushButton *button1 = new QPushButton(widget);
    button1->setText("1");
    button1->move(40, 445);
    button1->setFixedSize(70,45);


    widget->show();



    return app.exec();
}
