#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <string>
#include "MusyGen.h"
#include <QTimer>
#include <QTime>
#include <QString>
#include "MusyGen.h"
#include <thread>

class StopWatch;

namespace Ui {
class Mainwindow;
}

class Mainwindow : public QDialog
{
    std::thread thread1;
    MusyGen*musyGen;
    Q_OBJECT

public:
    explicit Mainwindow(QWidget *parent = nullptr);
    ~Mainwindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_confirm_clicked();

    void on_SetTime_clicked();

    void on_hours_clicked();

    void on_minutes_clicked();

    void on_seconds_clicked();

    void on_Infinite_clicked();

    void on_Start_clicked();

    void on_Stop_clicked();

    void on_Pause_clicked();

    void on_Export_clicked();

    void on_New_generate_clicked();

    void on_diff_order_clicked();

    void on_diff_time_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::Mainwindow *ui;
    QTimer * timer_ls;
    //s
};

#endif // MAINWINDOW_H
