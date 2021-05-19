//
// Created by Denis on 19/05/2021.
//

#include "Parser.h"

void Parser::ReadyInMidiFile(string& direct_path) {
    fstream my_file;
    my_file.open(direct_path, ios::in);
    if (!my_file) {
        cout << "No such file";
    }
    else {
        char ch;
        while (true) {
            my_file >> ch;
            if (my_file.eof())
                break;

            cout << ch;
        }

    }
    p_midifile=my_file;
    my_file.close();
}

void Parser::ParseFile(){
    map<int,vector<string>> parsefile;
    vector<string> trackcommands;
    string nut_in_hex;
    for(int track=0;track<p_midifile.getTrackCount();track++){
        trackcommands.clear();
        for (int event=0; event<p_midifile[track].size(); event++) {
           /* cout << dec << p_midifile[track][event].tick;
            cout << '\t' << dec << p_midifile[track][event].seconds;
            cout << '\t';
            if (p_midifile[track][event].isNoteOn())
                cout << p_midifile[track][event].getDurationInSeconds();
            cout << '\t' << hex;*/
            trackcommands={to_string(p_midifile[track][event].tick),to_string(p_midifile[track][event].seconds)};
            nut_in_hex.clear();
            for (int i=0; i<p_midifile[track][event].size(); i++)
               // cout << (int)p_midifile[track][event][i] << ' ';
                nut_in_hex+=to_string((int)p_midifile[track][event][i]);
            trackcommands.push_back(nut_in_hex);
            //cout << endl;
        }
        parsefile[track]=trackcommands;
    }
    Commands=parsefile;
}
