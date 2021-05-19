#include "MusyGen.h"


void MusyGen::importMidiFile(const std::string& filename)
{
	std::map<double, std::vector<Note*>> notes;

	input_midifile.read("../input_files/midi/" + filename + ".mid");
	input_midifile.doTimeAnalysis();
	input_midifile.linkNotePairs();

	int tracks = input_midifile.getTrackCount();

	// todo fix gangnam style
	if (tracks == 1)
	{
		input_midifile.splitTracks();
	}

	for (int track = 1; track < tracks; track++)
	{
//		std::cout << "Track " << track << std::endl << "**************************" << std::endl;

		int instrument = 0;
		// instrument
		for (int event = 0; event < input_midifile[track].size(); event++)
		{
			if (input_midifile[track][event].isTimbre())
			{
//				std::cout << "Instrument: " << input_midifile[track][event].getP1() << std::endl;
				instrument = input_midifile[track][event].getP1();
			}
		}

		double current_note_group_seconds = 0;
		bool is_first_note = true;
		for (int event = 0; event < input_midifile[track].size(); event++)
		{
			if (input_midifile[track][event].isNoteOn())
			{
//				std::cout << input_midifile[track][event].getTickDuration() << " NoteOn:  " << input_midifile[track][event].getKeyNumber() << ", "
//						  << input_midifile[track][event].getDurationInSeconds() << std::endl;

				Note* note = new Note(input_midifile[track][event].getKeyNumber(), input_midifile[track][event].getVelocity(),
						input_midifile[track][event].getDurationInSeconds(), 0,
						instrument);

				if (!is_first_note && input_midifile[track][event].seconds != current_note_group_seconds)
				{
					double delay = input_midifile[track][event].seconds - current_note_group_seconds;
					for (const auto& note_ : notes[current_note_group_seconds])
						note_->next_note_delay = delay;
					current_note_group_seconds = input_midifile[track][event].seconds;
				}
				else
				{
					current_note_group_seconds = input_midifile[track][event].seconds;
				}

				is_first_note = false;
				notes[input_midifile[track][event].seconds].push_back(note);
			}
//			if (input_midifile[track][event].isNoteOff())
//			{
//				std::cout << "NoteOff: " << input_midifile[track][event].getKeyNumber() << ", "
//						  << input_midifile[track][event].getDurationInSeconds() << std::endl;
//			}
		}
	}

	unsigned int total_bytes = 0;

	for (const auto& note_group : notes)
	{
		total_bytes += sizeof(note_group.first);

		for (const auto& note : note_group.second)
		{
			total_bytes += sizeof(*note);
		}
	}

	std::cout << "Total bytes ram " << total_bytes << std::endl;

	std::cout.precision(10);
	std::cout << "Total mb ram: " << (double)total_bytes / 1048576 << std::endl;
	std::cout << "Total gb ram: " << (double)total_bytes / 1073741824 << std::endl;

	trainMarkovModel(notes);
}

void MusyGen::exportMidiFile(const std::string& filename)
{
	std::ofstream outputfile("output.mid");
	outputfile << generated_midifile;
	outputfile.close();
}

void MusyGen::generateMusic(unsigned int duration)
{
	std::vector<Note*> current_group_of_notes = music_markov_chain.getRandomState();
	double curr_duration = 0;
	findMaxDuration(current_group_of_notes,curr_duration);
	addNotesToMedi(current_group_of_notes);

	while (curr_duration<(double) duration){
		current_group_of_notes = music_markov_chain.getNextState(current_group_of_notes);
		findMaxDuration(current_group_of_notes,curr_duration);
		addNotesToMedi(current_group_of_notes);
	}
}

void MusyGen::addNoteToMedi(const Note* note){
	bool found_track = false;
	int trackcount = generated_midifile.getTrackCount();
	for (int tracknr = 0; tracknr<trackcount; tracknr++){
		if (generated_midifile[tracknr].getEvent(0).getP1() == note->instrument ){
			found_track = true;

		}
	}
}

void MusyGen::addNotesToMedi(std::vector<Note*> notes){
	for (const Note* note : notes){
		addNoteToMedi(note);
	}
}

void MusyGen::findMaxDuration(const std::vector< Note*> notes, double& max){
 	double current_dur = max;
	for (const Note* note : notes){
		max = std::max((current_dur+note->duration),max);
	}
}

void MusyGen::changeVolume(const int _volume)
{
	volume = _volume;
}

void MusyGen::volumeArrow(const bool up)
{
	if (up){
		volume +=5;
	}
	else{
		volume -=5;
	}
}


void MusyGen::playMusicInfinitly()
{

//	bool playing = true;
//	while (playing)
//	{
//
//	}
}

void MusyGen::trainMarkovModel(const std::map<double, std::vector<Note*>>& notes)
{
	MarkovChain<std::vector<Note*>> markov_chain;

	if (order == 1)
	{
		std::map<std::vector<Note*>, std::map<std::vector<Note*>, int>> note_group_map;

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
			if (note_group_map[notes.rbegin()->second].find(notes.begin()->second) != note_group_map[notes.rbegin()->second].end())
				note_group_map[notes.rbegin()->second][notes.begin()->second]++;
			else
				note_group_map[notes.rbegin()->second][notes.begin()->second] = 1;
		}

		for (auto& note_group : notes)
		{
			for (auto& note : note_group.second)
			{
				delete note;
			}
		}

		std::map<std::vector<Note*>,int> sommen;

		for (const auto &it : note_group_map){
			if (!markov_chain.stateExists(it.first)) markov_chain.addState(it.first);
			sommen[it.first] = 0;
			for(const auto &trans : it.second){
				sommen[it.first] += trans.second;
			}
		}

		for (const auto &it : note_group_map){
			for(const auto &trans : it.second){
				markov_chain.setTransition(it.first,trans.first,((double)trans.second/sommen[it.first]));
			}
		}

		std::cout << "IsLegal: " << std::boolalpha << markov_chain.isLegal() << std::endl;
	}
	else
	{
		VariableMarkovChain<std::vector<Note*>> variable_markov_chain;



		MarkovChain<std::vector<std::vector<Note*>>> variable_markov_chain_first = variable_markov_chain.toFirstOrder();
	}
}
