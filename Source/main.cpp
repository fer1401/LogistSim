#include "Include/mainwindow.h"
#include "Welcomedialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WelcomeDialog welcome;

    if (welcome.exec() == QDialog::Accepted)
    {
        MainWindow w;
        w.show();

        return a.exec();

    } else {
        return 0;
    }
}
