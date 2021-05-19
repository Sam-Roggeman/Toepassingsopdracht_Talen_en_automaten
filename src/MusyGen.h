#ifndef MUSYGEN_MUSYGEN_H
#define MUSYGEN_MUSYGEN_H


#include "../libs/midifile/include/MidiFile.h"
#include "data_structures/Note.h"
#include "data_structures/MarkovChain.hpp"

class MusyGen
{
    MarkovChain<std::vector<Note>> music_markov_chain;
    unsigned int order = 1;
    smf::MidiFile input_midifile;
    smf::MidiFile generated_midifile;

public:
    MusyGen() = default;

    void importMidiFile(const std::string& filename);
    void exportMidiFile(const std::string& filename);

    void generateMusic(unsigned int duration);
    void playMusicInfinitly();

private:
	void trainMarkovModel(const std::vector<std::vector<Note>>& notes);
};


#endif //MUSYGEN_MUSYGEN_H
