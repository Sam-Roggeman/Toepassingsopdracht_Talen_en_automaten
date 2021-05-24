#ifndef MUSYGEN_MUSYGEN_H
#define MUSYGEN_MUSYGEN_H


#include "../libs/midifile/include/MidiFile.h"
#include "data_structures/Note.h"
#include "data_structures/MarkovChain.hpp"
#include "data_structures/VariableMarkovChain.hpp"
#include <iomanip>
//#include "../libs/midifile/include/RtMidi.h"

class MusyGen
{
public:
	smf::MidiFile input_midifile;
	std::map<int, std::vector<Note>> notes;
	int tracks = 0;
	int TPQ = 0;
	std::map<int, int> instrument_to_track_map;
	double tempo = 0;

	MarkovChain<std::vector<Note>> music_markov_chain;
	smf::MidiFile generated_midifile;

	unsigned int markov_order = 1;
	int volume = 50;

public:
	MusyGen();

	void importMidiFile(const std::string& filename);

	void exportMidiFile(const std::string& filename);

	void exportInputMidiFile(const std::string& filename);

	void setMarkovOrder(unsigned int _markov_order);

	void trainMarkovModel();

	void generateMusic(double duration);

	void playMusicInfinitly();

	void changeVolume(int _volume);

	void volumeArrow(bool up);

private:
	static int findMaxDuration(const std::vector<Note>& note_group);

	void notesToMidi(const std::map<int, std::vector<Note>>& generated_notes);
};


#endif //MUSYGEN_MUSYGEN_H
