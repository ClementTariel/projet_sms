#include <QApplication>
#include <QPushButton>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton bouton("l'emulateur de t9 n'est pas encore disponible voici un bouton a la place");

    bouton.show();

    return app.exec();
}
