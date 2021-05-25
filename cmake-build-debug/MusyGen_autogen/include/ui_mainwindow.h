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
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include "Stopwatch.h"

QT_BEGIN_NAMESPACE

class Ui_Mainwindow
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *page_2;
    QTextEdit *textEdit;
    QTextEdit *hallo;
    QPushButton *confirm;
    QWidget *page_3;
    QPushButton *Infinite;
    QPushButton *SetTime;
    QWidget *page_4;
    QDoubleSpinBox *doubleSpinBox;
    QPushButton *minutes;
    QPushButton *hours;
    QPushButton *seconds;
    StopWatch *widget;

    void setupUi(QDialog *Mainwindow)
    {
        if (Mainwindow->objectName().isEmpty())
            Mainwindow->setObjectName(QString::fromUtf8("Mainwindow"));
        Mainwindow->resize(720, 573);
        stackedWidget = new QStackedWidget(Mainwindow);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 721, 581));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        pushButton = new QPushButton(page);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(310, 290, 93, 28));
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(310, 340, 93, 28));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        textEdit = new QTextEdit(page_2);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(300, 240, 51, 51));
        hallo = new QTextEdit(page_2);
        hallo->setObjectName(QString::fromUtf8("hallo"));
        hallo->setGeometry(QRect(250, 170, 161, 51));
        hallo->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 255)"));
        confirm = new QPushButton(page_2);
        confirm->setObjectName(QString::fromUtf8("confirm"));
        confirm->setGeometry(QRect(280, 310, 93, 28));
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        Infinite = new QPushButton(page_3);
        Infinite->setObjectName(QString::fromUtf8("Infinite"));
        Infinite->setGeometry(QRect(310, 230, 93, 28));
        SetTime = new QPushButton(page_3);
        SetTime->setObjectName(QString::fromUtf8("SetTime"));
        SetTime->setGeometry(QRect(290, 280, 121, 31));
        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        doubleSpinBox = new QDoubleSpinBox(page_4);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setGeometry(QRect(340, 270, 62, 22));
        minutes = new QPushButton(page_4);
        minutes->setObjectName(QString::fromUtf8("minutes"));
        minutes->setGeometry(QRect(320, 330, 93, 28));
        hours = new QPushButton(page_4);
        hours->setObjectName(QString::fromUtf8("hours"));
        hours->setGeometry(QRect(210, 330, 93, 28));
        seconds = new QPushButton(page_4);
        seconds->setObjectName(QString::fromUtf8("seconds"));
        seconds->setGeometry(QRect(430, 330, 93, 28));
        widget = new StopWatch(page_4);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(180, 50, 331, 161));
        stackedWidget->addWidget(page_4);

        retranslateUi(Mainwindow);

        stackedWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(Mainwindow);
    } // setupUi

    void retranslateUi(QDialog *Mainwindow)
    {
        Mainwindow->setWindowTitle(QCoreApplication::translate("Mainwindow", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("Mainwindow", "Generate", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Mainwindow", "Exit", nullptr));
        textEdit->setHtml(QCoreApplication::translate("Mainwindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:7.8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        hallo->setHtml(QCoreApplication::translate("Mainwindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:7.8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Choose a markovchain order between 1 and 5.</p></body></html>", nullptr));
        confirm->setText(QCoreApplication::translate("Mainwindow", "Confirm", nullptr));
        Infinite->setText(QCoreApplication::translate("Mainwindow", "Play Infinite", nullptr));
        SetTime->setText(QCoreApplication::translate("Mainwindow", "Play for a Set Time", nullptr));
        minutes->setText(QCoreApplication::translate("Mainwindow", "In minutes", nullptr));
        hours->setText(QCoreApplication::translate("Mainwindow", "In hours", nullptr));
        seconds->setText(QCoreApplication::translate("Mainwindow", "In seconds", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Mainwindow: public Ui_Mainwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
