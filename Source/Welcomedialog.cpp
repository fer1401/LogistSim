#include "Welcomedialog.h"
#include "ui_welcomedialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);

    connect(ui->IniciarButton, &QPushButton::clicked, this, &WelcomeDialog::accept);

    connect(ui->SalirButton, &QPushButton::clicked, this, &WelcomeDialog::reject);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}
