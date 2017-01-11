
#include <iostream>

#include "include/UserInterface/mainwindow.h"
#include <QApplication>


int main(int argc, char* argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();

}
