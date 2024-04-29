#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

#include "fx.h"

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = nullptr);
    ~SettingsForm();

private slots:
    void on_cancel_btn_clicked();

    void on_save_btn_clicked();



    void on_delete_btn_clicked();

    void on_add_btn_clicked();

private:
    Ui::SettingsForm *ui;
};

#endif // SETTINGSFORM_H
