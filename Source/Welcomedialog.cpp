#include "Welcomedialog.h"
#include "ui_welcomedialog.h"
#include "Settings.h"

WelcomeDialog::WelcomeDialog(Simulation *s, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomeDialog), simulation (s)
{
    ui->setupUi(this);

    connect(ui->StartButton, &QPushButton::clicked, this, &WelcomeDialog::accept);
    connect(ui->ExitButton, &QPushButton::clicked, this, &WelcomeDialog::reject);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::on_SettingsButton_clicked()
{
    Settings *settingsWindow = new Settings(simulation, this);

    settingsWindow->show();
}
