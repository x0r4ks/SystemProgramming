#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fx.h"

#include <QMainWindow>
#include <QMediaPlayer>

#include <QAudioOutput>
#include <QListWidgetItem>
#include <QGraphicsPixmapItem>
#include <QFile>

#include "settingsform.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update_volume_lable(float value);

    void on_play_stop_btn_clicked(bool checked);



    void update_duration(qint64 dur);
    void update_position(qint64 dur);

    void on_actionOpen_Files_triggered();

    void on_prev_btn_clicked();

    void on_next_btn_clicked();

    void on_playlist_lv_itemDoubleClicked(QListWidgetItem *item);

    void on_rnd_btn_clicked(bool checked);


    void on_repiat_btn_clicked(bool checked);

    void on_actionOpen_Dir_triggered();

    void on_actionSettings_triggered();

    void on_mute_btn_clicked(bool checked);

    void on_actionvolumeUP_triggered();

    void on_actionvolumeDown_triggered();

    void on_actionmoveToLeft_triggered();

    void on_actionmoveToRight_triggered();

    void on_actionOpen_PlayList_triggered();

    void create_custom_menu(QPoint point);

    void on_actionSave_CUE_triggered();

    void on_actionSave_Portable_CUE_triggered();

private:
    Ui::MainWindow *ui;
    SettingsForm *sf;
    QMediaPlayer *player;
    QAudioOutput audio_out;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;

    QImage img;
    QSize old_size;

    bool randome_mode = false;
    bool repeat_mode = false;
    QString button_color_prefix = "";


    int play_list_count = -1;

    QVector<QString> paths;

    void play_by_id(int id);

    void update_button_icons_theme();


};
#endif // MAINWINDOW_H
