//
// Created by said2 on 25-5-2021.
//
//Gevonden op https://www.qtcentre.org/threads/55399-A-Stopwatch-in-qt-GUI
#ifndef MUSYGEN_STOPWATCH_H
#define MUSYGEN_STOPWATCH_H


#include <utility>
#include "QLabel"
#include "QHBoxLayout"
#include "QDialog"
#include "QString"
#include "QtWidgets"
#include "string"
#include "src/mainwindow.h"

class Mainwindow;

class StopWatch : public QWidget {
Q_OBJECT
public:
    explicit StopWatch(QWidget*parent = 0)
            : QWidget(parent)
            , mRunning(false)
            , mStartTime()
            , mLabel(new QLabel("00:00:00,000"))
            , mTotalTime(0)
            , mstop(false)
    {
        QGridLayout * gridLayout = new QGridLayout(this);

        gridLayout->addWidget(mLabel,   0, 0, 1, 3);

        QFont font("Arial", 24, QFont::Bold);
        QPalette palette = mLabel->palette();
        palette.setColor(QPalette::WindowText, Qt::blue);
        mLabel->setPalette(palette);
        mLabel->setFont(font);
        gridLayout->setAlignment(mLabel, Qt::AlignCenter);

        startTimer(0);
    }

public slots:
    bool getmstop(){
        return mstop;
    };
    void start(void)
    {
        mStartTime = QDateTime::currentDateTime();
        mRunning = true;
    }

    void pause(void)
    {
        timerEvent(new QTimerEvent(0));
        mTotalTime += mSessionTime;
        mRunning = false;
    }

    void stop(void)
    {
        mTotalTime = 0;
        mRunning = false;
        mSessionTime = 0;
        mstop = true;
    }

protected:
    void timerEvent(QTimerEvent *)
    {
        if(mRunning)
        {
            mSessionTime = mStartTime.msecsTo(QDateTime::currentDateTime());
            qint64 time = mTotalTime + mSessionTime;
            unsigned int h = time / 1000 / 60 / 60;
            unsigned int m = (time / 1000 / 60) - (h * 60);
            unsigned int s = (time / 1000) - (m * 60);
            unsigned int ms = time - (s + ((m + (h * 60))* 60)) * 1000;
            const QString diff = QString("%1:%2:%3,%4")
                    .arg(h,  2, 10, QChar('0'))
                    .arg(m,  2, 10, QChar('0'))
                    .arg(s,  2, 10, QChar('0'))
                    .arg(ms, 3, 10, QChar('0'));
            mLabel->setText(diff);
        }
    }

private:
    bool        mstop;
    bool        mRunning;
    QDateTime   mStartTime;
    QLabel *    mLabel;
    qint64      mTotalTime;
    qint64      mSessionTime;
};


#endif //MUSYGEN_STOPWATCH_H
