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
            , mStart(new QPushButton("Start"))
            , mPause(new QPushButton("Pause"))
            , mStop(new QPushButton("Stop"))
            , mOpened(true)
    {
        QGridLayout * gridLayout = new QGridLayout(this);

        gridLayout->addWidget(mLabel,   0, 0, 1, 3);
        gridLayout->addWidget(mStart,   1, 0, 1, 1);
        gridLayout->addWidget(mPause,   1, 1, 1, 1);
        gridLayout->addWidget(mStop,    1, 2, 1, 1);

        connect(mStart, SIGNAL(clicked()), SLOT(start()));
        connect(mPause, SIGNAL(clicked()), SLOT(pause()));
        connect(mStop, SIGNAL(clicked()), SLOT(stop()));

        QFont font("Arial", 24, QFont::Bold);
        QPalette palette = mLabel->palette();
        palette.setColor(QPalette::WindowText, Qt::blue);
        mLabel->setPalette(palette);
        mLabel->setFont(font);
        gridLayout->setAlignment(mLabel, Qt::AlignCenter);

        mStart->setEnabled(true);
        mPause->setEnabled(false);
        mStop->setEnabled(false);

        startTimer(0);
    }

    explicit StopWatch( Mainwindow*dialogs,std::string file, QWidget * parent = 0)
            : QWidget(parent)
            , mRunning(false)
            , mStartTime()
            , mLabel(new QLabel("00:00:00,000"))
            , mTotalTime(0)
            , mStart(new QPushButton("Start"))
            , mPause(new QPushButton("Pause"))
            , mStop(new QPushButton("Stop"))
            , mOpened(true)
            , mQdialog(dialogs)
            , mfilename(std::move(file))
    {
        QGridLayout * gridLayout = new QGridLayout(this);

        gridLayout->addWidget(mLabel,   0, 0, 1, 3);
        gridLayout->addWidget(mStart,   1, 0, 1, 1);
        gridLayout->addWidget(mPause,   1, 1, 1, 1);
        gridLayout->addWidget(mStop,    1, 2, 1, 1);

        connect(mStart, SIGNAL(clicked()), SLOT(start()));
        connect(mPause, SIGNAL(clicked()), SLOT(pause()));
        connect(mStop, SIGNAL(clicked()), SLOT(stop()));

        QFont font("Arial", 24, QFont::Bold);
        QPalette palette = mLabel->palette();
        palette.setColor(QPalette::WindowText, Qt::blue);
        mLabel->setPalette(palette);
        mLabel->setFont(font);
        gridLayout->setAlignment(mLabel, Qt::AlignCenter);

        mStart->setEnabled(true);
        mPause->setEnabled(false);
        mStop->setEnabled(false);

        startTimer(0);
    }

public slots:
    bool getMOpened(){
        return mOpened;
    };
    void start(void)
    {
        mStartTime = QDateTime::currentDateTime();
        mRunning = true;
        mStart->setEnabled(false);
        mPause->setEnabled(true);
        mStop->setEnabled(true);
    }

    void pause(void)
    {
        mStart->setEnabled(true);
        mPause->setEnabled(false);
        mStop->setEnabled(true);
        timerEvent(new QTimerEvent(0));
        mTotalTime += mSessionTime;
        mRunning = false;
        mOpened = false;
    }

    void stop(void)
    {
        mStart->setEnabled(true);
        mPause->setEnabled(false);
        mStop->setEnabled(false);
        mTotalTime = 0;
        mRunning = false;
        double totaltime = (double)mSessionTime/1000;

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
    bool        mOpened;
    bool        mRunning;
    QDateTime   mStartTime;
    QLabel *    mLabel;
    qint64      mTotalTime;
    qint64      mSessionTime;
    Mainwindow*    mQdialog;
    std::string         mfilename;

    QPushButton * mStart;
    QPushButton * mPause;
    QPushButton * mStop;
};


#endif //MUSYGEN_STOPWATCH_H
