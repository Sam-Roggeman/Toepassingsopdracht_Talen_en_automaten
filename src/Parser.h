//
// Created by Denis on 19/05/2021.
//

#ifndef MUSYGENTEST_PARSER_H
#define MUSYGENTEST_PARSER_H
#include "libs/midifile/include/MidiFile.h"
#include "string"
#include "iostream"
#include "map"
using namespace smf;
using namespace std;
class Parser {
    MidiFile p_midifile;
    map<int,vector<string>> Commands; // map[tracknummer]=vector(tick,seconds,note(in hexdecimal))
public:
    void ReadyInMidiFile(string& direct_path);

    void ParseFile();
};


#endif //MUSYGENTEST_PARSER_H
