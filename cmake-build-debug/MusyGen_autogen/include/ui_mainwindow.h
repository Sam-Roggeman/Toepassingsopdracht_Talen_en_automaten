/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.0.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <stopwatch.h>

QT_BEGIN_NAMESPACE

class Ui_Mainwindow
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *page_2;
    QPushButton *confirm;
    QSpinBox *order_keuze;
    QTextEdit *PlainTextEdit;
    QWidget *page_3;
    QPushButton *Infinite;
    QPushButton *SetTime;
    QWidget *page_4;
    QPushButton *minutes;
    QPushButton *seconds;
    QTextEdit *textEdit;
    QWidget *page_5;
    StopWatch *widget;
    QPushButton *Start;
    QPushButton *Stop;
    QPushButton *Pause;
    QSlider *verticalSlider;
    QWidget *page_6;
    QPushButton *Export;
    QWidget *page_7;
    QPushButton *New_generate;
    QPushButton *diff_order;
    QPushButton *diff_time;
    QPushButton *pushButton_4;
    QLineEdit *lineEdit;
    QPushButton *pushButton_5;

    void setupUi(QDialog *Mainwindow)
    {
        if (Mainwindow->objectName().isEmpty())
            Mainwindow->setObjectName(QString::fromUtf8("Mainwindow"));
        Mainwindow->resize(720, 573);
        stackedWidget = new QStackedWidget(Mainwindow);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 42, 720, 531));
        stackedWidget->setAutoFillBackground(true);
        stackedWidget->setStyleSheet(QString::fromUtf8(""));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        pushButton = new QPushButton(page);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(320, 260, 93, 28));
        pushButton->setStyleSheet(QString::fromUtf8("background-color = rgb(200,0,0)"));
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(320, 300, 93, 28));
        pushButton_2->setStyleSheet(QString::fromUtf8(""));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        confirm = new QPushButton(page_2);
        confirm->setObjectName(QString::fromUtf8("confirm"));
        confirm->setGeometry(QRect(340, 290, 93, 28));
        order_keuze = new QSpinBox(page_2);
        order_keuze->setObjectName(QString::fromUtf8("order_keuze"));
        order_keuze->setGeometry(QRect(340, 240, 42, 22));
        order_keuze->setMinimum(1);
        order_keuze->setMaximum(5);
        order_keuze->setValue(1);
        PlainTextEdit = new QTextEdit(page_2);
        PlainTextEdit->setObjectName(QString::fromUtf8("PlainTextEdit"));
        PlainTextEdit->setGeometry(QRect(293, 140, 131, 71));
        PlainTextEdit->setAutoFillBackground(true);
        PlainTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(232,228,228);"));
        PlainTextEdit->setReadOnly(true);
        stackedWidget->addWidget(page_2);
        order_keuze->raise();
        confirm->raise();
        PlainTextEdit->raise();
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        Infinite = new QPushButton(page_3);
        Infinite->setObjectName(QString::fromUtf8("Infinite"));
        Infinite->setGeometry(QRect(310, 230, 93, 28));
        SetTime = new QPushButton(page_3);
        SetTime->setObjectName(QString::fromUtf8("SetTime"));
        SetTime->setGeometry(QRect(290, 260, 131, 31));
        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        minutes = new QPushButton(page_4);
        minutes->setObjectName(QString::fromUtf8("minutes"));
        minutes->setGeometry(QRect(270, 330, 93, 28));
        seconds = new QPushButton(page_4);
        seconds->setObjectName(QString::fromUtf8("seconds"));
        seconds->setGeometry(QRect(380, 330, 93, 28));
        textEdit = new QTextEdit(page_4);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(320, 260, 104, 51));
        stackedWidget->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        widget = new StopWatch(page_5);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(180, 160, 331, 161));
        Start = new QPushButton(page_5);
        Start->setObjectName(QString::fromUtf8("Start"));
        Start->setGeometry(QRect(180, 330, 93, 28));
        Stop = new QPushButton(page_5);
        Stop->setObjectName(QString::fromUtf8("Stop"));
        Stop->setGeometry(QRect(410, 330, 93, 28));
        Pause = new QPushButton(page_5);
        Pause->setObjectName(QString::fromUtf8("Pause"));
        Pause->setGeometry(QRect(290, 330, 93, 28));
        verticalSlider = new QSlider(page_5);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        verticalSlider->setGeometry(QRect(540, 160, 22, 161));
        verticalSlider->setOrientation(Qt::Vertical);
        stackedWidget->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName(QString::fromUtf8("page_6"));
        Export = new QPushButton(page_6);
        Export->setObjectName(QString::fromUtf8("Export"));
        Export->setGeometry(QRect(300, 260, 93, 28));
        stackedWidget->addWidget(page_6);
        page_7 = new QWidget();
        page_7->setObjectName(QString::fromUtf8("page_7"));
        New_generate = new QPushButton(page_7);
        New_generate->setObjectName(QString::fromUtf8("New_generate"));
        New_generate->setGeometry(QRect(282, 210, 121, 28));
        diff_order = new QPushButton(page_7);
        diff_order->setObjectName(QString::fromUtf8("diff_order"));
        diff_order->setGeometry(QRect(280, 260, 121, 28));
        diff_time = new QPushButton(page_7);
        diff_time->setObjectName(QString::fromUtf8("diff_time"));
        diff_time->setGeometry(QRect(260, 320, 161, 28));
        pushButton_4 = new QPushButton(page_7);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(290, 370, 93, 28));
        stackedWidget->addWidget(page_7);
        lineEdit = new QLineEdit(Mainwindow);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(0, 0, 720, 51));
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(43,43,43);\n"
"color: rgb(255, 255, 255);\n"
""));
        lineEdit->setReadOnly(true);
        pushButton_5 = new QPushButton(Mainwindow);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(20, 10, 93, 28));
        lineEdit->raise();
        pushButton_5->raise();
        stackedWidget->raise();

        retranslateUi(Mainwindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Mainwindow);
    } // setupUi

    void retranslateUi(QDialog *Mainwindow)
    {
        Mainwindow->setWindowTitle(QCoreApplication::translate("Mainwindow", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("Mainwindow", "Import file", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Mainwindow", "Exit", nullptr));
        confirm->setText(QCoreApplication::translate("Mainwindow", "Confirm", nullptr));
        PlainTextEdit->setHtml(QCoreApplication::translate("Mainwindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:7.8pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Choose a markovchain order between 1 and 5.</p></body></html>", nullptr));
        Infinite->setText(QCoreApplication::translate("Mainwindow", "Play Infinitely", nullptr));
        SetTime->setText(QCoreApplication::translate("Mainwindow", "Export for a set time", nullptr));
        minutes->setText(QCoreApplication::translate("Mainwindow", "In minutes", nullptr));
        seconds->setText(QCoreApplication::translate("Mainwindow", "In seconds", nullptr));
        textEdit->setHtml(QCoreApplication::translate("Mainwindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:7.8pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0,00</p></body></html>", nullptr));
        Start->setText(QCoreApplication::translate("Mainwindow", "Start/Resume", nullptr));
        Stop->setText(QCoreApplication::translate("Mainwindow", "Stop", nullptr));
        Pause->setText(QCoreApplication::translate("Mainwindow", "Pause", nullptr));
        Export->setText(QCoreApplication::translate("Mainwindow", "Export", nullptr));
        New_generate->setText(QCoreApplication::translate("Mainwindow", "Import new file", nullptr));
        diff_order->setText(QCoreApplication::translate("Mainwindow", "Use different order", nullptr));
        diff_time->setText(QCoreApplication::translate("Mainwindow", "Choose a different time set", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Mainwindow", "Exit", nullptr));
        lineEdit->setText(QString());
        pushButton_5->setText(QCoreApplication::translate("Mainwindow", "DarkMode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Mainwindow: public Ui_Mainwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
