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

class StopWatch;

namespace Ui {
class Mainwindow;
}

class Mainwindow : public QDialog
{
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

private:
    Ui::Mainwindow *ui;
    QTimer * timer_ls;
    //s
};

#endif // MAINWINDOW_H
