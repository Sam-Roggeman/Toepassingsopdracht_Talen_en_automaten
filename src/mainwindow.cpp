#include "mainwindow.h"
#include "ui_mainwindow.h"


Mainwindow::Mainwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mainwindow)
{
    ui->setupUi(this);
}

Mainwindow::~Mainwindow()
{
    delete ui;
}

void Mainwindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"), "/home/jana", tr("Image Files ( *.mid)"));
    if(!fileName.isEmpty()){
        this->on_stackedWidget_currentChanged(1);
        musyGen = new MusyGen;
        std::string a = fileName.toStdString();
        musyGen->importMidiFile(a);
        ui->hallo->setReadOnly(true);
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
    QString a = this->ui->textEdit->toPlainText();
    musyGen->setMarkovOrder(std::stoi(a.toStdString()));
}

void Mainwindow::on_SetTime_clicked()
{
    this->on_stackedWidget_currentChanged(3);
}

void Mainwindow::on_hours_clicked()
{
    double uur = this->ui->doubleSpinBox->value();
    double aantal_sec = uur*3600;
    musyGen->generateMusic(aantal_sec);
    this->on_stackedWidget_currentChanged(5);
}

void Mainwindow::on_minutes_clicked()
{
    double minuten = this->ui->doubleSpinBox->value();
    double aantal_sec = minuten*60;
    musyGen->generateMusic(aantal_sec);
    this->on_stackedWidget_currentChanged(5);
}

void Mainwindow::on_seconds_clicked()
{
    double seconden = this->ui->doubleSpinBox->value();
    musyGen->generateMusic(seconden);
    this->on_stackedWidget_currentChanged(5);
}

void Mainwindow::on_Infinite_clicked()
{
    this->on_stackedWidget_currentChanged(4);
}
