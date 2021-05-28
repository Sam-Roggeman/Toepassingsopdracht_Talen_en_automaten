#include <cmath>
#include "MusyGen.h"


void MusyGen::importMidiFile(const std::string& filepath)
{
	if (FILE* file = fopen(filepath.c_str(), "r"))
	{
		fclose(file);
	}
	else
	{
		std::cerr << "INPUT FILE NOT FOUND" << std::endl;
		return;
	}
	input_midifile.clear();
	notes.clear();
	instrument_to_track_map.clear();

	input_midifile.read(filepath);

	input_midifile.absoluteTicks();
	input_midifile.doTimeAnalysis();
	input_midifile.linkNotePairs();
	TPQ = input_midifile.getTicksPerQuarterNote();
	tempo = 120;

	tracks = input_midifile.getTrackCount();

	if (tracks == 1)
	{
		for (int event = 0; event < input_midifile[0].size(); event++)
		{
			if (input_midifile[0][event].isTempo())
			{
				tempo = input_midifile[0][event].getTempoBPM();
			}
		}

		std::pair<int, int> trackinst;
		std::set<std::pair<int, int>> iset;
		for (int j = 0; j < input_midifile[0].getEventCount(); j++)
		{
			if (input_midifile[0][j].isTimbre())
			{
				trackinst.first = 0;
				trackinst.second = input_midifile[0][j].getP1();
				iset.insert(trackinst);
			}
		}
		int instrument = 0;

		int current_note_group_ticks = 0;
		bool is_first_note = true;
		for (int event = 0; event < input_midifile[0].size(); event++)
		{
			if (input_midifile[0][event].isTimbre())
			{
				instrument = input_midifile[0][event].getP1();
			}
			if (input_midifile[0][event].isNoteOn())
			{
				Note note(input_midifile[0][event].getKeyNumber(),
						input_midifile[0][event].getVelocity(),
						input_midifile[0][event].getTickDuration(), 0,
						instrument, 0, input_midifile[0][event].getChannel());

				if (!is_first_note && input_midifile[0][event].tick != current_note_group_ticks)
				{
					int delay = input_midifile[0][event].tick - current_note_group_ticks;

					for (auto& note_ : notes[current_note_group_ticks])
						note_.next_note_delay = delay;
					current_note_group_ticks = input_midifile[0][event].tick;
				}
				else
				{
					current_note_group_ticks = input_midifile[0][event].tick;
				}

				is_first_note = false;

				notes[input_midifile[0][event].tick].push_back(note);
			}
		}

	}
	else
	{
		for (int track = 0; track < tracks; track++)
		{
			for (int event = 0; event < input_midifile[track].size(); event++)
			{
				if (input_midifile[track][event].isTempo())
				{
					tempo = input_midifile[track][event].getTempoBPM();
				}
			}
		}

		std::pair<int, int> trackinst;
		std::set<std::pair<int, int>> iset;
		for (int i = 0; i < input_midifile.getTrackCount(); i++)
		{
			for (int j = 0; j < input_midifile[i].getEventCount(); j++)
			{
				if (input_midifile[i][j].isTimbre())
				{
					trackinst.first = i;
					trackinst.second = input_midifile[i][j].getP1();
					iset.insert(trackinst);
				}
			}
		}

		for (int track = 0; track < tracks; track++)
		{
			int instrument = 0;

			int current_note_group_ticks = 0;
			bool is_first_note = true;
			for (int event = 0; event < input_midifile[track].size(); event++)
			{
				if (input_midifile[track][event].isTimbre())
				{
					instrument = input_midifile[track][event].getP1();
				}
				if (input_midifile[track][event].isNoteOn())
				{
					Note note(input_midifile[track][event].getKeyNumber(),
							input_midifile[track][event].getVelocity(),
							input_midifile[track][event].getTickDuration(), 0,
							instrument, track, 0);

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
}

void MusyGen::exportMidiFile(const std::string& filepath)
{
	std::ofstream outputfile(filepath);
	generated_midifile.write(outputfile);
	outputfile.close();
}

void MusyGen::exportInputMidiFile(const std::string& filename)
{
	notesToMidi(notes);

//  USEFULL TO SEE DURATION BUT MESSES WITH THE FILE
//    std::cout << generated_midifile.getFileDurationInSeconds() << std::endl;

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

		// last and first
		if (notes.size() > 2)
		{
			if (note_group_map[notes.rbegin()->second].find(notes.begin()->second) !=
				note_group_map[notes.rbegin()->second].end())
				note_group_map[notes.rbegin()->second][notes.begin()->second]++;
			else
				note_group_map[notes.rbegin()->second][notes.begin()->second] = 1;
		}

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

		for (auto& state : note_group_map)
		{
			if (!music_markov_chain.stateExists(state.first)) music_markov_chain.addState(state.first);
		}

		for (auto& state : note_group_map)
		{
			int total_frequencies = 0;
			for (auto& frequency : state.second)
			{
				total_frequencies += frequency.second;
			}

			for (auto& frequency : state.second)
			{
				double probability = (double)frequency.second / total_frequencies;
				music_markov_chain.setTransition(state.first, frequency.first, probability);
			}
		}
	}
	else
	{
		VariableMarkovChain<std::vector<Note>> variable_markov_chain;
		variable_markov_chain.setOrder(markov_order);

		std::map<std::pair<std::vector<std::vector<Note>>, std::vector<Note>>, std::map<std::vector<Note>, int>> note_group_map;
		std::vector<std::vector<Note>> previous_note_groups;

		// create previous notes groups vector
		for (unsigned int i = markov_order - 1; i > 0; i--)
		{
			auto previous_note_group = std::next(notes.rbegin(), i - 1);
			previous_note_groups.push_back(previous_note_group->second);
		}

		for (auto note_group = notes.begin(); note_group != notes.end(); note_group++)
		{
			auto next_note_group = std::next(note_group, 1);

			if (next_note_group == notes.end())
			{
				continue;
			}

			if (note_group_map[std::make_pair(previous_note_groups, note_group->second)].find(
					(next_note_group)->second) !=
				note_group_map[std::make_pair(previous_note_groups, note_group->second)].end())
				note_group_map[std::make_pair(previous_note_groups, note_group->second)][(next_note_group)->second]++;
			else
				note_group_map[std::make_pair(previous_note_groups, note_group->second)][(next_note_group)->second] = 1;

			previous_note_groups.push_back(note_group->second);
			if (previous_note_groups.size() == markov_order)
				previous_note_groups.erase(previous_note_groups.begin());
		}

		// last and first
		if (notes.size() > 2)
		{
			if (note_group_map[std::make_pair(previous_note_groups, notes.rbegin()->second)].find(
					notes.begin()->second) !=
				note_group_map[std::make_pair(previous_note_groups, notes.rbegin()->second)].end())
				note_group_map[std::make_pair(previous_note_groups, notes.rbegin()->second)][notes.begin()->second]++;
			else
				note_group_map[std::make_pair(previous_note_groups, notes.rbegin()->second)][notes.begin()->second] = 1;
		}

		for (const auto& state : note_group_map)
		{
			if (!variable_markov_chain.stateExists(state.first.second))
				variable_markov_chain.addState(state.first.second);
		}

		for (const auto& state : note_group_map)
		{
			int total_frequencies = 0;
			for (const auto& frequency : state.second)
			{
				total_frequencies += frequency.second;
			}

			for (const auto& frequency : state.second)
			{
				double probability = (double)frequency.second / total_frequencies;
				variable_markov_chain.setTransition(state.first.second, { state.first.first }, frequency.first,
						probability);
			}
		}

		music_variable_to_first_chain = variable_markov_chain.toFirstOrder();
	}
}

int MusyGen::SecondsToTicks(double duration)
{
	double secondsPerTick = 60.0 / (tempo * TPQ);
	int total_ticks = (int)(duration / secondsPerTick);
	return total_ticks;
}

int MusyGen::TicksToSeconds(double ticks)
{
	double TicksPerSecond = (tempo * TPQ) / 60.0;
	int total_ticks = (int)(ticks / TicksPerSecond);
	return total_ticks;
}

double MusyGen::TicksToMs(double ticks)
{
	double TicksPerSecond = (tempo * TPQ) / 60.0;
	double total_ticks = ticks / TicksPerSecond;
	return total_ticks * 1000;
}


void MusyGen::generateMusic(double duration_in_seconds)
{
	double duration = SecondsToTicks(duration_in_seconds);

	if ((markov_order == 1 && music_markov_chain.empty()) ||
		(markov_order > 1 && music_variable_to_first_chain.empty()))
	{
		std::cerr << "Error: no Markov chain model" << std::endl;
		return;
	}

	std::map<int, std::vector<Note>> generated_notes;

	if (markov_order == 1)
	{
		std::vector<Note> current_group_of_notes = music_markov_chain.getRandomState();
		int curr_duration = 0;
		generated_notes[curr_duration] = current_group_of_notes;

//		curr_duration += findMaxDuration(current_group_of_notes);
//        curr_duration += findMinDuration(current_group_of_notes);
		curr_duration += findMinDelay(current_group_of_notes);

		while (curr_duration < duration)
		{
			current_group_of_notes = music_markov_chain.getNextState(current_group_of_notes);
			generated_notes[curr_duration] = current_group_of_notes;

//			curr_duration += findMaxDuration(current_group_of_notes);
//            curr_duration += findMinDuration(current_group_of_notes);
			curr_duration += findMinDelay(current_group_of_notes);
		}
	}
	else
	{
		std::vector<std::vector<Note>> current_state = music_variable_to_first_chain.getRandomState();
		std::vector<Note> current_group_of_notes = current_state.back();
		int curr_duration = 0;
		generated_notes[curr_duration] = current_group_of_notes;

//		curr_duration += findMaxDuration(current_group_of_notes);
//        curr_duration += findMinDuration(current_group_of_notes);
		curr_duration += findMinDelay(current_group_of_notes);

		while (curr_duration < duration)
		{
			current_state = music_variable_to_first_chain.getNextState(current_state);
			current_group_of_notes = current_state.back();
			generated_notes[curr_duration] = current_group_of_notes;
//			curr_duration += findMaxDuration(current_group_of_notes);
//            curr_duration += findMinDuration(current_group_of_notes);
			curr_duration += findMinDelay(current_group_of_notes);
		}
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

int MusyGen::findMinDuration(const std::vector<Note>& note_group)
{
	int min_duration = std::numeric_limits<int>::max();
	for (auto& note : note_group)
		min_duration = std::min(note.duration, min_duration);
	return min_duration;
}

void MusyGen::notesToMidi(const std::map<int, std::vector<Note>>& generated_notes)
{
	std::map<int, int> prev_ins_per_track;
//	for (int track = 1; track <tracks;track++) prev_ins_per_track[track] = 0;

	generated_midifile.clear();

	generated_midifile.setTicksPerQuarterNote(TPQ);

	// adds tracks with notes
	generated_midifile.addTracks(tracks - 1);

//	// adds timbre to tracks
//	for (const auto& instrument_track : instrument_to_track_map)
//	{
//		generated_midifile.addTimbre(instrument_track.second, 0, 0, instrument_track.first);
//		prev_ins_per_track[instrument_track.second] = instrument_track.first;
//	}

	// adds tempo
	generated_midifile.addTempo(0, 0, tempo);
	generated_midifile.addTrackName(0, 0, "Trackname");

	for (const auto& note_group : generated_notes)
	{
		int ins = 0;
		for (const auto& note : note_group.second)
		{
			int start_tick = note_group.first;
			int end_tick = note_group.first + note.duration;

			int track_nr = note.track;
			ins = note.instrument;


			if (prev_ins_per_track.find(track_nr) == prev_ins_per_track.end() || ins != prev_ins_per_track[track_nr])
			{
				generated_midifile.addTimbre(track_nr, start_tick, 0, ins);
				prev_ins_per_track[track_nr] = ins;
			}
			generated_midifile.addNoteOn(track_nr, start_tick, note.channel, note.key, note.velocity);
			generated_midifile.addNoteOff(track_nr, end_tick, note.channel, note.key, note.velocity);
		}
	}
	generated_midifile.sortTracks();
}

void MusyGen::playMusicInfinitely()
{
	unsigned int tempo_sleep = 0; //((3/tempo)*10000.00);
	smf::MidiMessage message = smf::MidiMessage();
	RtMidiOut* midiout = new RtMidiOut();

    //channel -> instrument
    std::map<int, int> current_active_instruments;
    //abs ticks -> notes
    std::map<unsigned int, const Note *> node_duration;
    // instrument van huidige node
    int instrument;
    //huidige tick
    unsigned int current_tick = 0;
    //aantal sleep - ticks
    unsigned int sleep_ticks;
    //aantal nodes dat gestopt is
    int todelete;
    //minimum van de huidige groep
    unsigned int minim;
    openPort(midiout);
    // Program change: 192, 5
    message.push_back(192);
    message.push_back(5);
    midiout->sendMessage(&message);
    SLEEP(500);
    message[0] = 0xF1;
    message[1] = 60;
    midiout->sendMessage(&message);
    volumeMessage(midiout);
    unsigned int next_grouptick = 0;

    std::map<unsigned int, const Note *>::iterator map_it;
    std::vector<Note> current_group_of_nodes;
    std::vector<std::vector<Note>> current_state;
    if (markov_order == 1 ) {
        current_group_of_nodes = music_markov_chain.getRandomState();
    }
    else {
        current_state = music_variable_to_first_chain.getRandomState();
        current_group_of_nodes = current_state.back();
    }

        while (playing_inf) {

            SLEEP(tempo_sleep);
            current_tick += tempo_sleep;
            todelete = 0;
            next_grouptick = findMinDelay(current_group_of_nodes);
            minim = findMinDelay(current_group_of_nodes) + current_tick;

            for (const auto &node : current_group_of_nodes) {
                instrument = node.instrument;
                node_duration[(double)node.duration  + current_tick] = &node; //*120/tempo
                if (volume_changed) {
                    volumeMessage(midiout);
                }
                if (current_active_instruments.find(node.track) == current_active_instruments.end() ||
                    current_active_instruments[node.track] == instrument) {
                    current_active_instruments[node.track] = instrument;
                    message.makeTimbre(node.track, instrument);
                    midiout->sendMessage(&message);
                }
                message.makeNoteOn(node.track, node.key, node.velocity);
                midiout->sendMessage(&message);
            }
            for (const auto &node:node_duration) {

                if (node.first >= minim) {
                    break;
                }
                if (current_tick < node.first) {
                    sleep_ticks = (node.first - current_tick);
                    SLEEP(TicksToMs((double)sleep_ticks)); //*120/tempo
//                    SLEEP(10 * 1000.00);
                    //                prev_tick = current_tick;

                } else {
                    message.makeNoteOff(node.second->track, node.second->key, node.second->velocity);
                    midiout->sendMessage(&message);

                    todelete++;
                }
                current_tick = std::max(node.first, current_tick);

            }
            if (markov_order == 1) {
                current_group_of_nodes = music_markov_chain.getNextState(current_group_of_nodes);
            }
            else if (markov_order > 1) {
                current_state = music_variable_to_first_chain.getNextState(current_state);
                current_group_of_nodes = current_state.back();
            }
            if (todelete > 0) {
                map_it = node_duration.begin();
                std::advance(map_it, todelete);
                node_duration.erase(node_duration.begin(), map_it);
            }
        }
cleanup:
    delete midiout;
    return;
}

void MusyGen::openPort(RtMidiOut* midiout)
{
	std::string portName;
	unsigned int i = 0, nPorts = midiout->getPortCount();
	if (nPorts == 0)
	{
		std::cout << "No output ports available!" << std::endl;
	}

	if (nPorts == 1)
	{
		std::cout << "\nOpening " << midiout->getPortName() << std::endl;
	}
	else
	{
		for (i = 0; i < nPorts; i++)
		{
			portName = midiout->getPortName(i);
			std::cout << "  Output port #" << i << ": " << portName << '\n';
		}

		do
		{
			std::cout << "\nChoose a port number: ";
			std::cin >> i;
		} while (i >= nPorts);
	}

	std::cout << "\n";
	midiout->openPort(i);
}


void MusyGen::volumeMessage(RtMidiOut* midiout)
{
	std::vector<unsigned char> message;
	//SysEx
	message.emplace_back(0xF0);
	//Realtime
    message.emplace_back(0x7F);
    //The SysEx channel. Could be from 0x00 to 0x7F.
    message.emplace_back(0x7F);
    //Sub-ID -- Device Control
    message.emplace_back(0x04);
    //Sub-ID2 -- Master Volume
    message.emplace_back(0x01);
    unsigned int LSB = volume & 0x7F;
    //Bits 0 to 6 of a 14-bit volume
    message.emplace_back(LSB);
    unsigned int MSB = volume & 0x3F80;
    MSB >>= 7;
    //Bits 7 to 13 of a 14-bit volume
    message.emplace_back(MSB);
    //End of SysEx
    message.emplace_back(0xF);

	volume_changed = false;
	midiout->sendMessage(&message);
}

void MusyGen::startNote(char key, char velocity, RtMidiOut* midiout)
{
	std::vector<unsigned char> message;
	message.emplace_back(0x90);
	message.emplace_back(key);
	message.emplace_back(velocity);

	midiout->sendMessage(&message);
}

void MusyGen::endNote(char key, char velocity, RtMidiOut* midiout)
{
	std::vector<unsigned char> message;
	message.emplace_back(0x80);
	message.emplace_back(key);
	message.emplace_back(velocity);

	midiout->sendMessage(&message);
}

void MusyGen::changeVolume(const int _volume)
{
	volume = (std::min(std::max(_volume,0),100)*((double)16383.00/100.00));
	volume_changed = true;
}

MusyGen::MusyGen()
{
	srand((unsigned)time(nullptr));
}

void MusyGen::setPlayInf(bool inf)
{
	playing_inf = inf;
}

int MusyGen::findMinDelay(std::vector<Note>& note_group)
{
    int count = 0;
    int delay = 0;
	int min = std::numeric_limits<int>::max();
	for (const Note& note:note_group)
	{
	    count ++;
	    delay += note.next_note_delay;
		min = std::min(note.next_note_delay, min);
	}
	return (min);
}