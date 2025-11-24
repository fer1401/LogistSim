#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

class Settings;

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

private slots:
    void on_settingsButton_clicked();

private:
    Ui::WelcomeDialog *ui;
    Settings *settingsWindow = nullptr;
};

#endif // WELCOMEDIALOG_H
