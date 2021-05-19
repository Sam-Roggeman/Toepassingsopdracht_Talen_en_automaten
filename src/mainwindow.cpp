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
    MusyGen musyGen;
    musyGen.importMidiFile(fileName.toStdString());
}

void Mainwindow::on_pushButton_2_clicked()
{
    this->close();
}
