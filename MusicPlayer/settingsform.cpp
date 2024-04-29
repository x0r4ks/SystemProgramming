#include "settingsform.h"
#include "ui_settingsform.h"


#include <QStandardPaths>
#include <QDir>
#include <QDirIterator>
#include <QMenu>
#include <QFileDialog>

SettingsForm::SettingsForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    QFileInfo f(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0)+"/"+PROGRAM_NAME+"/"+"music_list.txt");
    if (!f.exists()) {
        QDir dir;
        dir.mkdir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0)+"/"+PROGRAM_NAME);
        QFile f(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0)+"/"+PROGRAM_NAME+"/"+"music_list.txt");

        f.open(QIODevice::WriteOnly);

        f.write(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0).toStdString().c_str());

        f.close();
    } else {
        QFile f(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0)+"/"+PROGRAM_NAME+"/"+"music_list.txt");

        f.open(QIODevice::ReadOnly);

        QTextStream in(&f);
        ui->dirs_list->clear();

        while (!in.atEnd()) {
            QString line = in.readLine();

            ui->dirs_list->addItem(line);

        }
        f.close();
    }

}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::on_cancel_btn_clicked()
{
    this->close();
}


void SettingsForm::on_save_btn_clicked()
{
    QDir dir;
    dir.mkdir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0)+"/"+PROGRAM_NAME);
    QFile f(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0)+"/"+PROGRAM_NAME+"/"+"music_list.txt");
    f.open(QIODevice::WriteOnly);
    for (int i = 0; i < ui->dirs_list->count(); i++) {
        f.write(ui->dirs_list->item(i)->text().toStdString().c_str());
    }
    f.close();
    this->close();
}




void SettingsForm::on_delete_btn_clicked()
{
    QListWidgetItem *it = ui->dirs_list->takeItem(ui->dirs_list->currentRow());
    delete it;
}


void SettingsForm::on_add_btn_clicked()
{
    QString line = QFileDialog::getExistingDirectory(this, "Chose music Directory", QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0));
    ui->dirs_list->addItem(line);
}

