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

    widget->show();


    QPushButton bouton("l'emulateur de t9 n'est pas encore disponible voici un bouton a la place");
    bouton.show();

    return app.exec();
}
