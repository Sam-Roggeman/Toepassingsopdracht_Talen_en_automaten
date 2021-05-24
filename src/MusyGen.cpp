#include <cmath>
#include "MusyGen.h"


void MusyGen::importMidiFile(const std::string& filename)
{
	input_midifile.clear();
	notes.clear();
	instrument_to_track_map.clear();

	input_midifile.read("../input_files/midi/" + filename + ".mid");
	input_midifile.absoluteTicks();
	input_midifile.doTimeAnalysis();
	input_midifile.linkNotePairs();
	TPQ = input_midifile.getTicksPerQuarterNote();

	tracks = input_midifile.getTrackCount();

	// todo: add support for midi files of which all info is in track 0
	if (tracks == 1)
	{
		input_midifile.splitTracks();
	}

	// tempo
	for (int event = 0; event < input_midifile[0].size(); event++)
	{
		if (input_midifile[0][event].isTempo())
		{
			tempo = input_midifile[0][event].getTempoBPM();
			continue;
		}
	}

	for (int track = 1; track < tracks; track++)
	{
		// todo: fix instruments
		// instrument
		int instrument = 0;
		for (int event = 0; event < input_midifile[track].size(); event++)
		{
			if (input_midifile[track][event].isTimbre())
			{
				instrument = input_midifile[track][event].getP1();
				instrument_to_track_map[instrument] = track;
				continue;
			}
		}

		int current_note_group_ticks = 0;
		bool is_first_note = true;
		for (int event = 0; event < input_midifile[track].size(); event++)
		{
			if (input_midifile[track][event].isNoteOn())
			{
				Note note(input_midifile[track][event].getKeyNumber(),
						input_midifile[track][event].getVelocity(),
						input_midifile[track][event].getTickDuration(), 0,
						instrument);

				if (!is_first_note && input_midifile[track][event].tick != current_note_group_ticks)
				{
					int delay = input_midifile[track][event].tick - current_note_group_ticks;
					for (auto& note_ : notes[current_note_group_ticks])
						note_.next_note_delay = delay;
					current_note_group_ticks = input_midifile[track][event].tick;
				}
				else
				{
					current_note_group_ticks = input_midifile[track][event].tick;
				}

				is_first_note = false;
				notes[input_midifile[track][event].tick].push_back(note);
			}
		}
	}
}

void MusyGen::exportMidiFile(const std::string& filename)
{
	std::ofstream outputfile(filename + ".mid");
	generated_midifile.write(outputfile);
	outputfile.close();
}

void MusyGen::exportInputMidiFile(const std::string& filename)
{
	notesToMidi(notes);
	exportMidiFile(filename);
}

void MusyGen::setMarkovOrder(unsigned int _markov_order)
{
	markov_order = _markov_order;
}

void MusyGen::trainMarkovModel()
{
	music_markov_chain.clear();

	if (markov_order == 1)
	{
		std::map<std::vector<Note>, std::map<std::vector<Note>, int>> note_group_map;

		for (auto note_group = notes.begin(); note_group != notes.end(); note_group++)
		{
			auto nx = std::next(note_group, 1);

			if (nx == notes.end())
			{
				continue;
			}

			if (note_group_map[note_group->second].find((nx)->second) != note_group_map[note_group->second].end())
				note_group_map[note_group->second][(nx)->second]++;
			else
				note_group_map[note_group->second][(nx)->second] = 1;
		}

		// first and last
		if (notes.size() > 2)
		{
			if (note_group_map[notes.rbegin()->second].find(notes.begin()->second) !=
				note_group_map[notes.rbegin()->second].end())
				note_group_map[notes.rbegin()->second][notes.begin()->second]++;
			else
				note_group_map[notes.rbegin()->second][notes.begin()->second] = 1;
		}

		std::map<std::vector<Note>, int> frequencies;

		for (auto& it : note_group_map)
		{
			if (!music_markov_chain.stateExists(it.first)) music_markov_chain.addState(it.first);
			frequencies[it.first] = 0;
			for (const auto& trans : it.second)
			{
				frequencies[it.first] += trans.second;
			}
		}

		for (auto& it : note_group_map)
		{
			for (auto& trans : it.second)
			{
				music_markov_chain.setTransition(it.first, trans.first, ((double)trans.second / frequencies[it.first]));
			}
		}

		std::cout << "IsLegal: " << std::boolalpha << music_markov_chain.isLegal() << std::endl;
	}
	else
	{
		VariableMarkovChain<std::vector<Note*>> variable_markov_chain;

		// todo: variable order Markov chain training

		MarkovChain<std::vector<std::vector<Note*>>> variable_markov_chain_first = variable_markov_chain.toFirstOrder();
	}
}

void MusyGen::generateMusic(double duration)
{
	// todo: convert seconds to ticks

	if (music_markov_chain.empty())
	{
		std::cerr << "Error: no Markov chain model" << std::endl;
		return;
	}

	std::map<int, std::vector<Note>> generated_notes;

	std::vector<Note> current_group_of_notes = music_markov_chain.getRandomState();
	int curr_duration = 0;
	generated_notes[curr_duration] = current_group_of_notes;

	curr_duration += findMaxDuration(current_group_of_notes);

	while (curr_duration < duration)
	{
		current_group_of_notes = music_markov_chain.getNextState(current_group_of_notes);
		generated_notes[curr_duration] = current_group_of_notes;

		curr_duration += findMaxDuration(current_group_of_notes);
	}

	notesToMidi(generated_notes);
}

int MusyGen::findMaxDuration(const std::vector<Note>& note_group)
{
	int max_duration = 0;
	for (auto& note : note_group)
		max_duration = std::max(note.duration, max_duration);
	return max_duration;
}

void MusyGen::notesToMidi(const std::map<int, std::vector<Note>>& generated_notes)
{
	generated_midifile.clear();

	generated_midifile.setTicksPerQuarterNote(TPQ);

	// adds tracks with notes
	generated_midifile.addTracks(tracks - 1);

	// adds timbre to tracks
	for (const auto& instrument_track : instrument_to_track_map)
	{
		generated_midifile.addTimbre(instrument_track.second, 0, 0, instrument_track.first);
	}

	// adds tempo
	generated_midifile.addTempo(0, 0, tempo);
	generated_midifile.addTrackName(0, 0, "Trackname");

	for (const auto& note_group : notes)
	{
		for (const auto& note : note_group.second)
		{
			int start_tick = note_group.first;
			int end_tick = note_group.first + note.duration;

			int track_nr = instrument_to_track_map[note.instrument];
			generated_midifile.addNoteOn(track_nr, start_tick, 0, note.key, note.velocity);
			generated_midifile.addNoteOff(track_nr, end_tick, 0, note.key, note.velocity);
		}
	}

	generated_midifile.sortTracks();
}

void MusyGen::playMusicInfinitly()
{
//	bool playing = true;
//	while (playing)
//	{
//
//	}
}

void MusyGen::changeVolume(const int _volume)
{
	volume = _volume;
}

void MusyGen::volumeArrow(const bool up)
{
	if (up)
	{
		volume += 5;
	}
	else
	{
		volume -= 5;
	}
}

MusyGen::MusyGen()
{
	srand((unsigned)time(nullptr));
}
