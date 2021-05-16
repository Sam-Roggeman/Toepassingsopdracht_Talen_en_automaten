#ifndef MUSYGENTEST_MUSYGEN_H
#define MUSYGENTEST_MUSYGEN_H


#include "../libs/midifile/include/MidiFile.h"
#include "data_structures/Note.h"
#include "data_structures/MarkovChain.hpp"

class MusyGen
{
    MarkovChain<Note> music_markov_chain;
    smf::MidiFile midifile;

public:
    MusyGen() = default;

    void importMidiFile(const std::string& filename);
    void exportMidiFile(const std::string& filename);

    void generateMusic(unsigned int duration);


    void playMusicInfinitly();

};


#endif //MUSYGENTEST_MUSYGEN_H
