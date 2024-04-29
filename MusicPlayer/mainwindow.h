#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

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

    void on_actionOpen_triggered();

    void update_duration(qint64 dur);
    void update_position(qint64 dur);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QAudioOutput audio_out;



    QString path_to_file;
};
#endif // MAINWINDOW_H
