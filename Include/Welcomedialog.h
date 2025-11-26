#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include "Simulation.h"
#include "mainwindow.h"

class Settings;

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(Simulation *s, QWidget *parent = nullptr);
    ~WelcomeDialog();

private slots:


    void on_SettingsButton_clicked();

    void on_StartButton_clicked();

    void on_ExitButton_clicked();

private:
    Ui::WelcomeDialog *ui;
    Simulation *simulation;
};

#endif // WELCOMEDIALOG_H
