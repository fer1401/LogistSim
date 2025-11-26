#include "Welcomedialog.h"
#include "ui_welcomedialog.h"
#include "Settings.h"

WelcomeDialog::WelcomeDialog(Simulation *s, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomeDialog), simulation (s)
{
    ui->setupUi(this);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::on_SettingsButton_clicked()
{
    Settings *settingsWindow = new Settings(simulation);

    settingsWindow->show();

    this->close();
}

void WelcomeDialog::on_StartButton_clicked()
{
    MainWindow *mainWindow = new MainWindow(simulation);

    mainWindow->show();

    this->close();
}

void WelcomeDialog::on_ExitButton_clicked()
{
    this->close();
}

