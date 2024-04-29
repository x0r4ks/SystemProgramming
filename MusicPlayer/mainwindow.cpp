#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->player = new QMediaPlayer();
    this->player->setAudioOutput(&audio_out);
    audio_out.setVolume(0.5);

    connect(&audio_out, &QAudioOutput::volumeChanged, this, &MainWindow::update_volume_lable);
    connect(ui->volume_slider, &QSlider::valueChanged, this, [this](){
        this->audio_out.setVolume(((float)ui->volume_slider->value())/100);
    });
    connect(this->player, &QMediaPlayer::durationChanged, this, &MainWindow::update_duration);
    connect(this->player, &QMediaPlayer::positionChanged, this, &MainWindow::update_position);

    connect(this->player, &QMediaPlayer::positionChanged, ui->duration_slider, &QSlider::setValue);
    connect(ui->duration_slider, &QSlider::sliderMoved, this->player, &QMediaPlayer::setPosition);


}

MainWindow::~MainWindow()
{
    delete player;
    delete ui;
}

void MainWindow::update_volume_lable(float value)
{
    ui->volume_lable->setText("Volume " + QString::number(value*100) + "% :");
}

void MainWindow::on_play_stop_btn_clicked(bool checked)
{

    if (checked) {
        ui->play_stop_btn->setIcon(QIcon(":/icons/contorl/icons/play.png"));
        this->player->pause();
    } else {
        ui->play_stop_btn->setIcon(QIcon(":/icons/contorl/icons/stop.png"));
        this->player->play();
    }


}


void MainWindow::on_actionOpen_triggered()
{
    this->path_to_file.clear();
    this->path_to_file = QFileDialog::getOpenFileName(this,
                                                      "Chose audio file",
                                                      QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0),
                                                      "Audio files (*.mp3 *.flac *.wav *.ogg);;All files (*.*)");

    if (!this->path_to_file.isEmpty() || !this->path_to_file.isNull()) {




        QFileInfo f(this->path_to_file);
        this->ui->music_file_name->setText(f.fileName());

        try {
            this->player->setSource(QUrl::fromLocalFile(this->path_to_file));
            ui->play_stop_btn->click();
            this->player->play();

        }  catch (const std::exception &err) {
            QMessageBox::critical(this, "Error", err.what());
        }
    }
}

void MainWindow::update_duration(qint64 dur)
{
    int sec = dur / 1000;
    int min = sec / 60;
    sec %= 60;

    ui->duration_lbl->setText(QString::number(min) + ":" + QString::number(sec));
    ui->duration_slider->setMaximum(dur);
    ui->duration_slider->setMinimum(0);
}

void MainWindow::update_position(qint64 dur)
{
    int sec = dur / 1000;
    int min = sec / 60;
    sec %= 60;
    ui->curent_duration_lbl->setText(QString::number(min) + ":" + ((sec < 10)?"0":"") + QString::number(sec));
}

