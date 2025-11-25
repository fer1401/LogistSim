#include "Include/mainwindow.h"
#include "Welcomedialog.h"
#include "Simulation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Simulation simulation(nullptr);

    WelcomeDialog welcome(&simulation);

    welcome.show();

    return a.exec();
}
