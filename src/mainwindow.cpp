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
        MusyGen musyGen;
        std::string a = fileName.toStdString();
//    musyGen.importMidiFile(a);
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
