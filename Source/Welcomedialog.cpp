#include "Welcomedialog.h"
#include "ui_welcomedialog.h"
#include "Settings.h"

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);

    connect(ui->StartButton, &QPushButton::clicked, this, &WelcomeDialog::accept);

    connect(ui->ExitButton, &QPushButton::clicked, this, &WelcomeDialog::reject);

    connect(ui->SettingsButton, &QPushButton::clicked, this, &WelcomeDialog::on_settingsButton_clicked);

}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::on_settingsButton_clicked()
{
    // 1. VERIFICAR: Si el puntero es nulo, la ventana NO ha sido creada.
    if (settingsWindow == nullptr)
    {
        // Crear la ventana por primera y única vez
        settingsWindow = new Settings(this);

        // Es una ventana independiente
        settingsWindow->setWindowFlag(Qt::Window);

        // Opcional: Si quieres que al cerrarse la ventana, el puntero se resetee
        // y se pueda crear una nueva en el futuro, conecta la señal finished.
        connect(settingsWindow, &QObject::destroyed, this, [this]() {
            settingsWindow = nullptr;
        });

        // La línea settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
        // es correcta para asegurar que la memoria se libere.
        // Pero si usas la conexión de arriba, puedes omitir la conexión 'finished'
        // y usar 'deleteLater' en un slot, pero el puntero debe ser nullptr de todas formas.
        // Mantenemos WA_DeleteOnClose:
        settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    }

    // 2. MOSTRAR/ACTIVAR: Muestra la ventana. Si ya está visible, la trae al frente.
    settingsWindow->show();
    settingsWindow->activateWindow();
}
