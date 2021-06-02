#include "../libs/midifile/include/MidiFile.h"
#include "../libs/midifile/include/Options.h"
#include "data_structures/MarkovChain.hpp"
#include "data_structures/VariableMarkovChain.hpp"
#include "data_structures/Note.h"
#include "MusyGen.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Mainwindow w;
    w.show();
    return a.exec();
}
