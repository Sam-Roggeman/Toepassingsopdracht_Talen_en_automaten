#ifndef MUSYGEN_MUSYGEN_H
#define MUSYGEN_MUSYGEN_H



#include <limits.h>
#include "../libs/midifile/include/MidiFile.h"
#include "../libs/midifile/include/MidiMessage.h"
#include "data_structures/Note.h"
#include "data_structures/MarkovChain.hpp"
#include "data_structures/VariableMarkovChain.hpp"
#include <iomanip>
#include "../libs/RTMidi//include/RtMidi.h"

// Platform-dependent sleep routines.
#if defined(WIN32)
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants
#include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

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
	MarkovChain<std::vector<std::vector<Note>>> music_variable_to_first_chain;
	smf::MidiFile generated_midifile;

	unsigned int markov_order = 1;
	int volume = 50;
	bool volume_changed = false;

public:
	MusyGen();

	void importMidiFile(const std::string& filepath);

	void exportMidiFile(const std::string& filepath);

	void exportInputMidiFile(const std::string& filename);

	void setMarkovOrder(unsigned int _markov_order);

	void trainMarkovModel();

	void generateMusic(double duration_in_seconds);

	/**
	 * Als referencie voor de MIDI Output
	 * https://www.music.mcgill.ca/~gary/rtmidi/index.html
	 */
	void playMusicInfinitly();

	void changeVolume(int _volume);


private:
	static int findMaxDuration(const std::vector<Note>& note_group);

	static int findMinDuration(const std::vector<Note>& note_group);

	void notesToMidi(const std::map<int, std::vector<Note>>& generated_notes);

    int SecondsToTicks(double duration);

    int TicksToSeconds(double ticks);

    void startNote(char key, char velocity, RtMidiOut *midiout);

    void endNote(char key, char velocity, RtMidiOut *midiout);

    void volumeMessage(RtMidiOut *midiout);

    double TicksToSecondsDouble(double ticks);
};


#endif //MUSYGEN_MUSYGEN_H
