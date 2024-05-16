#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QDirIterator>
#include <QPixmap>
#include <QMediaMetaData>
#include <QListWidget>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->player = new QMediaPlayer();
    this->player->setAudioOutput(&audio_out);
    audio_out.setVolume(0.5);

    ui->playlist_lv->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->playlist_lv, &QListWidget::customContextMenuRequested, this, &MainWindow::create_custom_menu);

    connect(&audio_out, &QAudioOutput::volumeChanged, this, &MainWindow::update_volume_lable);
    connect(ui->volume_slider, &QSlider::valueChanged, this, [this](){
        this->audio_out.setVolume(((float)ui->volume_slider->value())/100);
        if (this->audio_out.isMuted()) {
            ui->mute_btn->click();
        }
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
        try {
            QMediaMetaData md = this->player->metaData();
            ui->music_file_name->setText(md.value(QMediaMetaData::Title).toString());
            ui->albom_name_lbl->setText(md.value(QMediaMetaData::AlbumTitle).toString());
            ui->group_name_lbl->setText(md.value(QMediaMetaData::ContributingArtist).toString());




            this->setWindowTitle("MusicPlayer: " + md.value(QMediaMetaData::Title).toString());

            if (md.value(QMediaMetaData::AudioCodec).toString() == "MP3") {
                img = md.value(QMediaMetaData::ThumbnailImage).view<QImage>();
                scene = new QGraphicsScene();

                ui->image_prev->setScene(scene);

                img = img.scaled(ui->image_prev->width(), ui->image_prev->height(), Qt::KeepAspectRatio);

                item = new QGraphicsPixmapItem(QPixmap::fromImage(img));

                scene->addItem(item);

                old_size = ui->image_prev->size();
            } else {
                img = QImage(":/icons/icons/vinyl.jpeg");
                scene = new QGraphicsScene();

                ui->image_prev->setScene(scene);

                img = img.scaled(ui->image_prev->width(), ui->image_prev->height(), Qt::KeepAspectRatio);

                item = new QGraphicsPixmapItem(QPixmap::fromImage(img));

                scene->addItem(item);

                old_size = ui->image_prev->size();
            }


        } catch (const std::exception& err) {
            QMessageBox::warning(this, "Warning", err.what());
        }
    });

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

        while (!in.atEnd()) {
            QString line = in.readLine();

            QDirIterator it(line, QStringList(), QDir::Files, QDirIterator::Subdirectories);
            QStringList qsl;

            while (it.hasNext())
                qsl << it.next();

            QStringList suffix;
            suffix << "mp3" << "wav" << "flac" << "ogg";

            for (int i = 0; i < qsl.size(); i++) {
                if (!paths.contains(qsl.at(i))) {
                    QFileInfo f(qsl.at(i));
                    if (suffix.contains(f.suffix())) {
                        ui->playlist_lv->addItem(f.fileName());
                        paths.push_back(qsl.at(i));
                    }

                }
            }

        }
        f.close();
    }

    // ui->actionmoveToLeft->setVisible(false);
    // ui->actionmoveToRight->setVisible(false);
    // ui->actionvolumeDown->setVisible(false);
    // ui->actionvolumeUP->setVisible(false);

    button_color_prefix = "light";


    update_button_icons_theme();
    ui->play_stop_btn->setIcon(QIcon(":/" + button_color_prefix + "/icons/" + button_color_prefix +"/play.png"));

}

MainWindow::~MainWindow()
{
    delete item;
    delete scene;
    delete player;
    delete sf;
    delete ui;
}

void MainWindow::update_volume_lable(float value)
{
    ui->volume_lable->setText("Volume " + QString::number(value*100) + "% :");
}

void MainWindow::on_play_stop_btn_clicked(bool checked)
{

    if (checked) {
        update_button_icons_theme();
        this->player->pause();
    } else {
        update_button_icons_theme();
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

    if (ui->image_prev->size() != old_size) {
        this->player->metaDataChanged();
    }

}


void MainWindow::on_actionOpen_Files_triggered()
{
    QVector<QString> np = QFileDialog::getOpenFileNames(this,
                                 "Chose audio file",
                                 QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0),
                                 "Audio files (*.mp3 *.flac *.wav *.ogg);;All files (*.*)");

    ui->playlist_lv->clear();
    paths.clear();




    for (int i = 0; i < np.size(); i++) {
        if (!paths.contains(np.at(i))) {
            QFileInfo f(np.at(i));
            ui->playlist_lv->addItem(f.fileName());
            qDebug() << np.at(0);
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
    this->player->setSource(QUrl::fromLocalFile(paths.at(id)));

    ui->playlist_lv->setCurrentItem(ui->playlist_lv->item(id));
    if (ui->play_stop_btn->isChecked())
        ui->play_stop_btn->click();


    this->player->play();
}

void MainWindow::update_button_icons_theme()
{
    ui->next_btn->setIcon(QIcon(":/" + button_color_prefix + "/icons/" + button_color_prefix +"/next-track.png"));
    ui->prev_btn->setIcon(QIcon(":/" + button_color_prefix + "/icons/" + button_color_prefix +"/previous-track.png"));
    ui->repiat_btn->setIcon(QIcon(":/" + button_color_prefix + "/icons/" + button_color_prefix +"/loop.png"));
    ui->rnd_btn->setIcon(QIcon(":/" + button_color_prefix + "/icons/" + button_color_prefix +"/random.png"));

    if (audio_out.isMuted()) {
        ui->mute_btn->setIcon(QIcon(":/" + button_color_prefix + "/icons/" + button_color_prefix +"/mute.png"));
    } else {
        ui->mute_btn->setIcon(QIcon(":/" + button_color_prefix + "/icons/" + button_color_prefix +"/unmute.png"));
    }

    if (player->isPlaying()) {
        ui->play_stop_btn->setIcon(QIcon(":/" + button_color_prefix + "/icons/" + button_color_prefix +"/play.png"));
    } else {
        ui->play_stop_btn->setIcon(QIcon(":/" + button_color_prefix + "/icons/" + button_color_prefix +"/stop.png"));
    }
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
    update_button_icons_theme();
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
        this->player->setLoops(1);

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
                ui->playlist_lv->addItem(f.fileName());
                paths.push_back(qsl.at(i));
            }

        }
    }

    if (paths.size() > 0) {
        play_list_count = 0;
        play_by_id(play_list_count);
    }
}



void MainWindow::on_actionSettings_triggered()
{

    sf = new SettingsForm();
    sf->show();

}


void MainWindow::on_mute_btn_clicked(bool checked)
{
    this->audio_out.setMuted(checked);

    if (checked) {
        update_button_icons_theme();
    } else {
        update_button_icons_theme();
    }

}


void MainWindow::on_actionvolumeUP_triggered()
{
    ui->volume_slider->setValue(ui->volume_slider->value()+5);
}


void MainWindow::on_actionvolumeDown_triggered()
{
    ui->volume_slider->setValue(ui->volume_slider->value()-5);
}


void MainWindow::on_actionmoveToLeft_triggered()
{
    this->player->setPosition(this->player->position() - 1000);
}


void MainWindow::on_actionmoveToRight_triggered()
{
    this->player->setPosition(this->player->position() + 1000);
}


void MainWindow::on_actionOpen_PlayList_triggered()
{
    QString file = QFileDialog::getOpenFileName(this,
                                          "Chose audio file",
                                          QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0),
                                          "Cue files (*.cue);;");

    QVector<QString> musicList;

    QFile f(file);
    QFileInfo info_file(file);
    QString path_prefix=info_file.path();

    f.open(QIODevice::ReadOnly);
    if (f.isOpen()) {
        while(!f.atEnd()) {
            QString line = f.readLine();
            if (line.count("FILE") == 1) {
                QVector<QString> pp = line.split("\"");
                if (!pp.at(1).contains("/")) {
                    musicList.push_back(path_prefix + "/" + pp.at(1));
                } else {
                    musicList.push_back(pp.at(1));
                }
            }
        }
    } f.close();


    ui->playlist_lv->clear();
    paths.clear();
    for (int i = 0; i < musicList.size(); i++) {
        if (!paths.contains(musicList.at(i))) {
            QFileInfo ff(musicList.at(i));
            if (ff.exists()) {
                ui->playlist_lv->addItem(ff.fileName());
                paths.push_back(musicList.at(i));
            }
        }
    }
}

void MainWindow::create_custom_menu(QPoint point)
{
    QPoint globalPos = ui->playlist_lv->mapToGlobal(point);


    QMenu myMenu;
    myMenu.addAction("Add", this, [this](){
        QVector<QString> np = QFileDialog::getOpenFileNames(this,
                                                            "Chose audio file",
                                                            QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0),
                                                            "Audio files (*.mp3 *.flac *.wav *.ogg);;All files (*.*)");

        for (int i = 0; i < np.size(); i++) {
            if (!paths.contains(np.at(i))) {
                QFileInfo f(np.at(i));
                ui->playlist_lv->addItem(f.fileName());
                qDebug() << np.at(0);
                paths.push_back(np.at(i));
            }
        }

    });
    myMenu.addAction("Delete",  this, [this](){
        int row = ui->playlist_lv->currentRow();
        QListWidgetItem *item = ui->playlist_lv->takeItem(row);
        delete item;
        paths.removeAt(row);
    });

    myMenu.addAction("Clear",  this, [this](){ui->playlist_lv->clear(); paths.clear();});

    myMenu.exec(globalPos);
}


void MainWindow::on_actionSave_CUE_triggered()
{
    QString file_path = QFileDialog::getSaveFileName(this,
                                                     "Save CUE",
                                                     QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0),
                                                     "CUE files (*.cue);;All files (*.*)");


    QFile f(file_path);
    if (f.open(QIODevice::WriteOnly)) {
        f.write("REM GENRE uncknow\n");
        f.write("REM DATE 1970\n");
        f.write("REM DISCID 00000000\n");
        f.write("REM COMMENT \"MediaPlayer playlist file\"\n");
        f.write("PERFORMER \"UNKNOW\"\n");
        f.write("TITLE \"UNKNOW\"\n");

        for (QString path : paths) {
            f.write("FILE \"");
            f.write(path.toStdString().c_str());
            f.write("\" WAVE\n");
        }
        f.close();
    }
}


void MainWindow::on_actionSave_Portable_CUE_triggered()
{
    QString file_path = QFileDialog::getSaveFileName(this,
                                                     "Save CUE",
                                                     QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0),
                                                     "CUE files (*.cue);;All files (*.*)");
    QFileInfo fI(file_path);
    QString dir_name = fI.fileName();
    QDir dir;
    dir.mkdir(fI.path() +"/"+ fI.fileName().split(".")[0]);
    QString save_path = fI.path() +"/"+ fI.fileName().split(".")[0];

    QFile f(save_path +"/"+ fI.fileName());
    if (f.open(QIODevice::WriteOnly)) {
        f.write("REM GENRE uncknow\n");
        f.write("REM DATE 1970\n");
        f.write("REM DISCID 00000000\n");
        f.write("REM COMMENT \"MediaPlayer playlist file\"\n");
        f.write("PERFORMER \"UNKNOW\"\n");
        f.write("TITLE \"UNKNOW\"\n");

        for (QString path : paths) {
            QFileInfo Fi(path);
            f.write("FILE \"");
            f.write(Fi.fileName().toStdString().c_str());
            f.write("\" WAVE\n");

            QFile::copy(path, fI.path() +"/"+ fI.fileName().split(".")[0] + "/" + Fi.fileName());
        }
        f.close();
    }
}

