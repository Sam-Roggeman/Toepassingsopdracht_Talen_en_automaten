#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <chrono>
#include <thread>


Mainwindow::Mainwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mainwindow)
{
    ui->setupUi(this);
    this->setFixedSize(720,573);
    setWindowTitle("MusyGen");
    QString qString("../Icon/Icon.png");
    QIcon qIcon(qString);
    setWindowIcon(qIcon);

    ui->pushButton->move(getnewpoint(ui->pushButton,-40,this->width(),this->height(),0));
    ui->pushButton_2->move(getnewpoint(ui->pushButton_2,0,this->width(),this->height(),0));
    ui->Infinite->move(getnewpoint(ui->pushButton,-40,this->width(),this->height(),0));
    ui->SetTime->move(getnewpoint(ui->pushButton_2,0,this->width(),this->height(),-15));
    ui->Export->move(getnewpoint(ui->pushButton,0,this->width(),this->height(),0));
    ui->New_generate->move(getnewpoint(ui->pushButton_2,-80,this->width(),this->height(),-15));
    ui->diff_order->move(getnewpoint(ui->pushButton_2,-40,this->width(),this->height(),-15));
    ui->diff_time->move(getnewpoint(ui->pushButton_2,0,this->width(),this->height(),-35));
    ui->pushButton_4->move(getnewpoint(ui->pushButton_2,40,this->width(),this->height(),0));
    ui->order_keuze->move(getSpinboxPoint(ui->order_keuze,0,this->width(),this->height(),0));
    ui->PlainTextEdit->move(getPlainTextPoint(ui->PlainTextEdit,-60,this->width(),this->height(),0));
    ui->confirm->move(getnewpoint(ui->confirm,50,this->width(),this->height(),-1));
    ui->textEdit->move(getPlainTextPoint(ui->textEdit,-30,this->width(),this->height(),0));
    ui->minutes->move(getnewpoint(ui->minutes,40,this->width(),this->height(),-50));
    ui->seconds->move(getnewpoint(ui->seconds,40,this->width(),this->height(),50));
    ui->widget->move(getStopwatchpoint(ui->widget,-30,this->width(),this->height(),0));
    ui->Pause->move(ui->Pause->x()+20,ui->Pause->y());
    ui->Start->move(ui->Start->x()+30,ui->Start->y());
    ui->PlainTextEdit->setFrameStyle(QFrame::NoFrame);
    ui->PlainTextEdit->viewport()->setAutoFillBackground(false);
}

Mainwindow::~Mainwindow()
{
    delete ui;
}

void Mainwindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open midi file"), "/home/jana", tr("Midi file ( *.mid)"));
    if(!fileName.isEmpty()){
        this->on_stackedWidget_currentChanged(1);
        musyGen = new MusyGen;
        std::string a = fileName.toStdString();
        musyGen->importMidiFile(a);
        ui->PlainTextEdit->setReadOnly(true);
    }

}

void Mainwindow::on_pushButton_2_clicked()
{
    this->close();
}

void Mainwindow::on_stackedWidget_currentChanged(int arg1)
{
    this->ui->stackedWidget->setCurrentIndex(arg1);
}

void Mainwindow::on_confirm_clicked()
{
    this->on_stackedWidget_currentChanged(2);
    musyGen->setMarkovOrder(this->ui->order_keuze->value());
    musyGen->trainMarkovModel();
}

void Mainwindow::on_SetTime_clicked()
{
    this->on_stackedWidget_currentChanged(3);
}

void Mainwindow::on_minutes_clicked()
{
    QString qString = this->ui->textEdit->toPlainText();
    std::string string = qString.toStdString();
    double minuten =  std::stod(string);
    double aantal_sec = minuten*60;
    musyGen->generateMusic(aantal_sec);
    this->on_stackedWidget_currentChanged(5);
}

void Mainwindow::on_seconds_clicked()
{
    QString qString = this->ui->textEdit->toPlainText();
    std::string string = qString.toStdString();
    double seconden = std::stod(string);
    musyGen->generateMusic(seconden);
    this->on_stackedWidget_currentChanged(5);
}

void Mainwindow::on_Infinite_clicked()
{
    this->on_stackedWidget_currentChanged(4);
    ui->Start->setEnabled(true);
    ui->Pause->setEnabled(false);
    ui->Stop->setEnabled(false);
    ui->widget->setmstarttime(QDateTime::currentDateTime());
    ui->verticalSlider->setSliderPosition(75);
    musyGen->changeVolume(ui->verticalSlider->sliderPosition());
}

void Mainwindow::on_Start_clicked()
{
    ui->widget->start();
    ui->Start->setEnabled(false);
    ui->Pause->setEnabled(true);
    ui->Stop->setEnabled(true);
    if(!musyGen->playing_inf){
        thread1 = std::thread(&MusyGen::playMusicInfinitely,musyGen);
    }
    musyGen->setPlayInf(true);
    musyGen->setPause(false);
}

void Mainwindow::on_Stop_clicked()
{
    ui->widget->stop();
    musyGen->setPause(false);
    musyGen->setPlayInf(false);
    thread1.join();

    this->on_stackedWidget_currentChanged(6);

}

void Mainwindow::on_Pause_clicked()
{
    ui->widget->pause();
    ui->Start->setEnabled(true);
    ui->Pause->setEnabled(false);
    musyGen->setPause(true);
}

void Mainwindow::on_Export_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save music file"), "/home/jana",
                                                    tr("Midi file ( *.mid)"));
    if(!fileName.isEmpty()){
        this->on_stackedWidget_currentChanged(6);
        musyGen->exportMidiFile(fileName.toStdString());
    }
}

void Mainwindow::on_New_generate_clicked()
{
    on_stackedWidget_currentChanged(0);
}

void Mainwindow::on_diff_order_clicked()
{
    on_stackedWidget_currentChanged(1);
}

void Mainwindow::on_diff_time_clicked()
{
    on_stackedWidget_currentChanged(2);
}

void Mainwindow::on_pushButton_4_clicked()
{
    this->close();
}

void Mainwindow::on_pushButton_5_clicked()
{
    if(dark){
        ui->pushButton->setStyleSheet("QPushButton { background-color: grey; }\n"
                                      "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->widget->setStyleSheet("QPushButton { background-color: grey; }\n"
                                  "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->pushButton_2->setStyleSheet("QPushButton { background-color: grey; }\n"
                                        "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->pushButton_4->setStyleSheet("QPushButton { background-color: grey; }\n"
                                        "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->Stop->setStyleSheet("QPushButton { background-color: grey; }\n"
                                "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->Start->setStyleSheet("QPushButton { background-color: grey; }\n"
                                 "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->stackedWidget->setStyleSheet("QPushButton { background-color: grey; }\n"
                                         "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->Export->setStyleSheet("QPushButton { background-color: grey; }\n"
                                  "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->Infinite->setStyleSheet("QPushButton { background-color: grey; }\n"
                                    "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->New_generate->setStyleSheet("QPushButton { background-color: grey; }\n"
                                        "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->Pause->setStyleSheet("QPushButton { background-color: grey; }\n"
                                 "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->SetTime->setStyleSheet("QPushButton { background-color: grey; }\n"
                                   "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->confirm->setStyleSheet("QPushButton { background-color: grey; }\n"
                                   "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->diff_order->setStyleSheet("QPushButton { background-color: grey; }\n"
                                      "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->minutes->setStyleSheet("QPushButton { background-color: grey; }\n"
                                   "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->seconds->setStyleSheet("QPushButton { background-color: grey; }\n"
                                   "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->lineEdit->setStyleSheet("QLineEdit { background-color: grey; }\n"
                                    "QLineEdit::enabled { background-color: rgb(43,43,43); color: rgb(255, 255, 255);}\n");
        ui->stackedWidget->setStyleSheet("QStackedWidget { background-color: grey; }\n"
                                         "QStackedWidget::enabled { background-color: rgb(240,240,240);}\n");
        ui->pushButton_5->setStyleSheet("QPushButton { background-color: grey; }\n"
                                        "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->diff_time->setStyleSheet("QPushButton { background-color: grey; }\n"
                                     "QPushButton:enabled { background-color: rgb(232,228,228); color: rgb(0, 0, 0);}\n");
        ui->PlainTextEdit->setStyleSheet("QTextEdit::enabled {color : rgb(0,0,0);}\n");
        QString qString("../Icon/Icon.png");
        QIcon qIcon(qString);
        setWindowIcon(qIcon);
        dark = false;
        light = true;
        QString qString1("DarkMode");
        ui->pushButton_5->setText(qString1);
    }
    else{

        ui->pushButton->setStyleSheet("QPushButton { background-color: grey; }\n"
                                      "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->widget->setStyleSheet("QPushButton { background-color: grey; }\n"
                                  "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->pushButton_2->setStyleSheet("QPushButton { background-color: grey; }\n"
                                        "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->pushButton_4->setStyleSheet("QPushButton { background-color: grey; }\n"
                                        "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->Stop->setStyleSheet("QPushButton { background-color: grey; }\n"
                                "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->Start->setStyleSheet("QPushButton { background-color: grey; }\n"
                                 "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->stackedWidget->setStyleSheet("QPushButton { background-color: grey; }\n"
                                         "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->Export->setStyleSheet("QPushButton { background-color: grey; }\n"
                                  "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->Infinite->setStyleSheet("QPushButton { background-color: grey; }\n"
                                    "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->New_generate->setStyleSheet("QPushButton { background-color: grey; }\n"
                                        "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->Pause->setStyleSheet("QPushButton { background-color: grey; }\n"
                                 "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->SetTime->setStyleSheet("QPushButton { background-color: grey; }\n"
                                   "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->confirm->setStyleSheet("QPushButton { background-color: grey; }\n"
                                   "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->diff_order->setStyleSheet("QPushButton { background-color: grey; }\n"
                                      "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->minutes->setStyleSheet("QPushButton { background-color: grey; }\n"
                                   "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->seconds->setStyleSheet("QPushButton { background-color: grey; }\n"
                                   "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->lineEdit->setStyleSheet("QPushButton { background-color: grey; }\n"
                                    "QPushButton:enabled { background-color: rgb(240,240,240); color: rgb(255, 255, 255);}\n");
        ui->stackedWidget->setStyleSheet("QStackedWidget { background-color: grey; }\n"
                                         "QStackedWidget::enabled { background-color: rgb(43,43,43);}\n");
        ui->pushButton_5->setStyleSheet("QPushButton { background-color: grey; }\n"
                                        "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->diff_time->setStyleSheet("QPushButton { background-color: grey; }\n"
                                     "QPushButton:enabled { background-color: rgb(7,132,181); color: rgb(255, 255, 255);}\n");
        ui->PlainTextEdit->setStyleSheet("QTextEdit::enabled {color : rgb(255,255,255);}\n");
        QString qString("../Icon/Icon2.png");
        QIcon qIcon(qString);
        setWindowIcon(qIcon);
        dark = true;
        light = false;
        QString qString1("LightMode");
        ui->pushButton_5->setText(qString1);
    }

}

void Mainwindow::on_verticalSlider_valueChanged(int value)
{
    musyGen->changeVolume(value);

}

QPoint Mainwindow::getnewpoint(QPushButton *button,int offset,int width, int height, int xoffset) {
    int dx = button->width()/2;
    int dy = button->height()/2;
    QPoint qPoint((width/2)-dx+xoffset,(height/2)-dy+offset);
    return qPoint;
}

QPoint Mainwindow::getStopwatchpoint(StopWatch *button, int offset, int width, int height, int xoffset) {
    int dx = button->width()/2;
    int dy = button->height()/2;
    QPoint qPoint((width/2)-dx+xoffset,(height/2)-dy+offset);
    return qPoint;
}

QPoint Mainwindow::getSpinboxPoint(QSpinBox *button, int offset, int width, int height, int xoffset) {
    int dx = button->width()/2;
    int dy = button->height()/2;
    QPoint qPoint((width/2)-dx+xoffset,(height/2)-dy+offset);
    return qPoint;
}

QPoint Mainwindow::getPlainTextPoint(QTextEdit *button, int offset, int width, int height, int xoffset) {
    int dx = button->width()/2;
    int dy = button->height()/2;
    QPoint qPoint((width/2)-dx+xoffset,(height/2)-dy+offset);
    return qPoint;
}

