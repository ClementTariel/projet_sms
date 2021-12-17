#include <QApplication>
#include <QKeyEvent>
#include "KeyPress.hpp"
#include "common.hpp"
#include <stdlib.h>
#include <iostream>

KeyPress::KeyPress(QWidget *parent) :
    QWidget(parent)
{

}

void KeyPress::keyPressEvent(QKeyEvent *ev)
{
    //myLabelText->setText("You Pressed Key " + ev->text());
}

void KeyPress::keyReleaseEvent(QKeyEvent *ev)
{
    //myLabelText->setText("You Release Key " + ev->text());
    QString inLineEditDisplay = ev->text();
    for (int i=0; i<inLineEditDisplay.count();i++){
        global_buffer[global_buffer_pos] = inLineEditDisplay.toStdString()[i];
        global_buffer_pos++;
    }
    std::cout << global_buffer_pos << "\n";
    std::cout << global_buffer << "\n";
}
