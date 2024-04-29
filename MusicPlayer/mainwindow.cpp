#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QDirIterator>
#include <QPixmap>
#include <QMediaMetaData>



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


    connect(this->player, &QMediaPlayer::playbackStateChanged, this, [this](){
        if (this->player->playbackState() == QMediaPlayer::StoppedState && player->position() == player->duration()) {
            ui->next_btn->click();
            ui->duration_slider->clearFocus();
        }
    });

    connect(this->player, &QMediaPlayer::metaDataChanged, this, [this](){
        QMediaMetaData md = this->player->metaData();
        ui->music_file_name->setText(md.value(QMediaMetaData::Title).toString());
        QImage img = md.value(QMediaMetaData::ThumbnailImage).view<QImage>();
        scene = new QGraphicsScene();

        // img= img.scaled(ui->image_prev->size().width(), ui->image_prev->size().height(), Qt::KeepAspectRatio);


        ui->image_prev->setScene(scene);

        img = img.scaled(ui->image_prev->width(), ui->image_prev->height(), Qt::KeepAspectRatio);

        item = new QGraphicsPixmapItem(QPixmap::fromImage(img));

        scene->addItem(item);


    });
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

void MainWindow::update_duration(qint64 dur)
{
    int sec = dur / 1000;
    int min = sec / 60;
    sec %= 60;

    ui->duration_lbl->setText(QString::number(min) + ":" + ((sec < 10)?"0":"") + QString::number(sec));
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


void MainWindow::on_actionOpen_Files_triggered()
{
    // paths.clear();
    // ui->playlist_lv->clear();

    QVector<QString> np = QFileDialog::getOpenFileNames(this,
                                 "Chose audio file",
                                 QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0),
                                 "Audio files (*.mp3 *.flac *.wav *.ogg);;All files (*.*)");

    for (int i = 0; i < np.size(); i++) {
        if (!paths.contains(np.at(i))) {
            QFileInfo f(np.at(i));
            ui->playlist_lv->addItem(QString::number(i+1) + " " + f.fileName());

            paths.push_back(np.at(i));
        }
    }

    play_list_count = 0;

    play_by_id(play_list_count);


}


void MainWindow::on_prev_btn_clicked()
{
    play_list_count-=1;
    if (play_list_count < 0) {
        play_list_count = this->paths.size() - 1;
    }
    play_by_id(play_list_count);
}

void MainWindow::play_by_id(int id)
{
    this->player->setSource(paths.at(id));

    ui->playlist_lv->setCurrentItem(ui->playlist_lv->item(id));
    if (ui->play_stop_btn->isChecked())
        ui->play_stop_btn->click();


    this->player->play();

    // ui->music_file_name->setText(ui->playlist_lv->currentItem()->text());

}


void MainWindow::on_next_btn_clicked()
{
    if (!randome_mode)
    {
        play_list_count+=1;
        if (play_list_count >= this->paths.size()) {
            play_list_count = 0;
        }


    } else {

        int range = this->paths.size() - 1 - 0 + 1;
        int num ;
        do {
            num = rand() % range + 0;
        }
        while (num == play_list_count);
        play_list_count = num;

    }

    play_by_id(play_list_count);
}


void MainWindow::on_playlist_lv_itemDoubleClicked(QListWidgetItem *item)
{
    play_list_count = ui->playlist_lv->currentRow();
    play_by_id(play_list_count);
}


void MainWindow::on_rnd_btn_clicked(bool checked)
{
    this->randome_mode = checked;
}

void MainWindow::on_repiat_btn_clicked(bool checked)
{
    if (checked) {
        this->player->setLoops(2);
    } else {
        this->player->setLoops(0);

    }
}

void MainWindow::on_actionOpen_Dir_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this, "Chose music Directory", QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0));

    QDirIterator it(path, QStringList(), QDir::Files, QDirIterator::Subdirectories);
    QStringList qsl;

    while (it.hasNext())
        qsl << it.next();

    QStringList suffix;
    suffix << "mp3" << "wav" << "flac" << "ogg";

    for (int i = 0; i < qsl.size(); i++) {
        if (!paths.contains(qsl.at(i))) {
            QFileInfo f(qsl.at(i));
            if (suffix.contains(f.suffix())) {
                ui->playlist_lv->addItem(QString::number(i+1) + " " + f.fileName());
                paths.push_back(qsl.at(i));
            }

        }
    }

    if (paths.size() > 0) {
        play_list_count = 0;
        play_by_id(play_list_count);
    }
}





