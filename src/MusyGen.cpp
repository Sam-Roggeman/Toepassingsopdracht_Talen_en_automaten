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
    this->changeVolume(50);
	input_midifile.clear();
	notes.clear();

	input_midifile.read(filepath);

	input_midifile.absoluteTicks();
	input_midifile.doTimeAnalysis();
	input_midifile.linkNotePairs();
	TPQ = input_midifile.getTicksPerQuarterNote();

	bool multitrack = false;
	if (input_midifile.getTrackCount() > 1)
	{
		input_midifile.joinTracks();
		multitrack = true;
	}

	std::vector<std::pair<int, int>> controllers;
	std::map<std::pair<int, int>, int> current_instruments;
	double current_tempo = 0;

	int tempo_count = 0;
	double tempo_sum = 0;

	int current_note_group_ticks = 0;
	bool is_first_note = true;

	for (int event = 0; event < input_midifile[0].size(); event++)
	{
		// instruments
		if (input_midifile[0][event].isTimbre())
		{
			current_instruments[std::make_pair(input_midifile[0][event].track,
					input_midifile[0][event].getChannel())] = input_midifile[0][event].getP1();
//			current_instrument = input_midifile[0][event].getP1();
		}

		// tempo
		if (input_midifile[0][event].isTempo())
		{
			current_tempo = input_midifile[0][event].getTempoBPM();
			tempo_count++;
			tempo_sum += current_tempo;
		}

		// controllers
		if (input_midifile[0][event].isController())
		{
			controllers.emplace_back(std::make_pair(
					input_midifile[0][event].getControllerNumber(),
					input_midifile[0][event].getControllerValue()
			));
		}

		// notes
		if (input_midifile[0][event].isNoteOn())
		{
			Note note(input_midifile[0][event].getKeyNumber(),
					input_midifile[0][event].getVelocity(),
					input_midifile[0][event].getTickDuration(), 0,
					current_instruments[std::make_pair(input_midifile[0][event].track,
							input_midifile[0][event].getChannel())],
					input_midifile[0][event].track, input_midifile[0][event].getChannel(),
					(int)current_tempo, controllers);
			controllers.clear();

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

	tempo = tempo_sum / tempo_count;

	if (multitrack)
		input_midifile.splitTracks();
}

void MusyGen::exportMidiFile(const std::string& filepath)
{
	generated_midifile.write(filepath);
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

int MusyGen::SecondsToTicks(double duration) const
{
	double secondsPerTick = 60.0 / (tempo * TPQ);
	int total_ticks = (int)(duration / secondsPerTick);
	return total_ticks;
}

int MusyGen::TicksToSeconds(double ticks) const
{
	double TicksPerSecond = (tempo * TPQ) / 60.0;
	int total_ticks = (int)(ticks / TicksPerSecond);
	return total_ticks;
}

double MusyGen::TicksToMs(double ticks) const
{
    return 1000*(ticks / ((tempo * TPQ) / 60.0));
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

//		curr_duration += findMinDelay(current_group_of_notes);
		curr_duration += current_group_of_notes.front().next_note_delay;

		while (curr_duration < duration)
		{
			current_group_of_notes = music_markov_chain.getNextState(current_group_of_notes);
			generated_notes[curr_duration] = current_group_of_notes;

			curr_duration += findMinDelay(current_group_of_notes);
		}
	}
	else
	{
		std::vector<std::vector<Note>> current_state = music_variable_to_first_chain.getRandomState();
		std::vector<Note> current_group_of_notes = current_state.back();
		int curr_duration = 0;
		generated_notes[curr_duration] = current_group_of_notes;

//		curr_duration += findMinDelay(current_group_of_notes);
		curr_duration += current_group_of_notes.front().next_note_delay;

		while (curr_duration < duration)
		{
			current_state = music_variable_to_first_chain.getNextState(current_state);
			current_group_of_notes = current_state.back();
			generated_notes[curr_duration] = current_group_of_notes;

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
	generated_midifile.clear();

	generated_midifile.setTicksPerQuarterNote(TPQ);

	// adds tracks with notes
	generated_midifile.addTracks(input_midifile.getTrackCount() - 1);

	// adds tempo
	generated_midifile.addTempo(0, 0, tempo);

	std::map<std::pair<int, int>, int> current_instruments;
	int current_tempo = 0;

	for (const auto& note_group : generated_notes)
	{
		for (const auto& note : note_group.second)
		{
			int start_tick = note_group.first;
			int end_tick = note_group.first + note.duration;

			int track_nr = note.track;

			// instruments
			auto track_channel_pair = std::make_pair(track_nr, note.channel);
			if (current_instruments.find(track_channel_pair) == current_instruments.end() ||
				current_instruments[track_channel_pair] != note.instrument)
			{
				generated_midifile.addTimbre(track_nr, start_tick, note.channel, note.instrument);
				current_instruments[track_channel_pair] = note.instrument;
			}

			// tempo
			if (note.tempo != current_tempo)
			{
				current_tempo = note.tempo;
				generated_midifile.addTempo(track_nr, start_tick, current_tempo);
			}

			// controllers before note
			if (!note.controllers.empty())
			{
				for (const auto& controller : note.controllers)
				{
					generated_midifile.addController(track_nr, start_tick, note.channel, controller.first,
							controller.second);
				}
			}

			// notes
			generated_midifile.addNoteOn(track_nr, start_tick, note.channel, note.key, note.velocity);
			generated_midifile.addNoteOff(track_nr, end_tick, note.channel, note.key, note.velocity);
		}
	}
	generated_midifile.sortTracks();
}

void MusyGen::playMusicInfinitely()
{



	smf::MidiMessage message = smf::MidiMessage();
	auto* midiout = new RtMidiOut;
    int port = findPort(midiout);
    midiout->openPort(port);
    short int type = input_midifile.getType();
    short int channel;
    //channel -> instrument
    std::map<int, int> current_active_instruments;
    //abs ticks -> notes
    std::map<unsigned int, std::vector<Note>> node_duration;
    // instrument van huidige node
    int instrument;
    //huidige tick
    unsigned int current_tick = 0;
    //aantal sleep - ticks
    unsigned int sleep_ticks = 0;
    //aantal nodes dat gestopt is
    int todelete = 0;
    //minimum van de huidige groep
    unsigned int minim = 0;
    bool sleep_to_node;
//    // Program change: 192, 5
//    message.push_back(192);
//    message.push_back(5);
//    midiout->sendMessage(&message);
//    SLEEP(500);
//    message[0] = 0xF1;
//    message[1] = 60;
//    midiout->sendMessage(&message);
    volumeMessage(midiout);


    std::map<unsigned int, std::vector<Note>>::iterator map_it;
    std::vector<Note> current_group_of_nodes;
    std::vector<std::vector<Note>> current_state;

//    for (const auto& track : this->track_controllers){
//        for (const auto& controller : track.second)
//        {
//            message.makeController(track.first,controller.first,controller.second);
//            midiout->sendMessage(&message);
//
//        }
//    }

    if (markov_order == 1 ) {
        current_group_of_nodes = music_markov_chain.getRandomState();
    }
    else {
        current_state = music_variable_to_first_chain.getRandomState();
        current_group_of_nodes = current_state.back();
    }

    while (playing_inf) {
        todelete = 0;
        minim = findMinDelay(current_group_of_nodes) + current_tick;
        if (paused_inf){
            node_duration.clear();
            delete midiout;
            while (paused_inf){
                if (!playing_inf){
                    goto cleanup;
                }
                SLEEP(200);
            }
            midiout = new RtMidiOut;
            midiout->openPort(port);
        }
        for (const auto &node : current_group_of_nodes) {
            channel = node.getInfChannel(type);
            instrument = node.instrument;
            if (volume_changed) {
                delete midiout;
                midiout = new RtMidiOut;
                midiout->openPort(port);
                volumeMessage(midiout);
            }
            if (current_active_instruments.find(channel) == current_active_instruments.end() ||
                current_active_instruments[channel] != instrument) {
                current_active_instruments[channel] = instrument;
                message.makeTimbre(channel, instrument);
                midiout->sendMessage(&message);
            }
            node_duration[(node.duration + current_tick)].emplace_back( node);
            for (const auto& controller : node.controllers)
                {
                    message.makeController(channel,controller.first,controller.second);
                    midiout->sendMessage(&message);
                }
            message.makeNoteOn(channel, node.key, node.velocity);
            midiout->sendMessage(&message);
        }
        for (auto i = node_duration.begin(), last = node_duration.end(); i != last; ) {
            sleep_to_node = i->first <= minim;
            //als de note gestopt moet worden
            if (i->first < current_tick || sleep_to_node) {
                if (sleep_to_node){
                    sleep_ticks = i->first - current_tick;
                    SLEEP(TicksToMs(sleep_ticks));
                    current_tick += sleep_ticks;
                }
                for (const auto &node:i->second) {
                    channel = node.getInfChannel(type);
                    message.makeNoteOff(channel, node.key);
                    midiout->sendMessage(&message);
                }
                i = node_duration.erase(i);

            } else break;
        }
        if (current_tick < minim) {
            sleep_ticks = (minim - current_tick);
            SLEEP(TicksToMs( sleep_ticks));
            current_tick += sleep_ticks;
        }

        if (markov_order == 1) {
            current_group_of_nodes = music_markov_chain.getNextState(current_group_of_nodes);
        } else if (markov_order > 1) {
            current_state = music_variable_to_first_chain.getNextState(current_state);
            current_group_of_nodes = current_state.back();
        }
        for (const auto &notes_v:node_duration) {
        }
//        if (todelete > 0) {
//            map_it = node_duration.begin();
//            std::advance(map_it, todelete);
//            node_duration.erase(node_duration.begin(), map_it);
//        }
    }
cleanup:
    delete midiout;
}

int MusyGen::findPort(RtMidiOut* midiout)
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
	return i;
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
	volume = (int)(std::min(std::max(_volume, 0), 100) * (16383.00 / 100.00));
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
	int min_delay = std::numeric_limits<int>::max();
	for (const Note& note:note_group)
	{
		min_delay = std::min(note.next_note_delay, min_delay);
	}
	return min_delay;
}

void MusyGen::pauseMessage(RtMidiOut *pOut) {
    std::vector<unsigned char> message;
    message.resize(0);
    message.emplace_back(0b11111100);
    pOut->sendMessage(&message);
    for (int i = 0; i<16;i++ ) {
        message.resize(0);
        unsigned char a = 0b10111111 & i;
        message.emplace_back(a);
        a = 0xFF & 120;
        message.emplace_back(a);
        a = 0xFF & 0;
        message.emplace_back(a);
        pOut->sendMessage(&message);
    }
}
void MusyGen::startMessage(RtMidiOut *pOut) {
    std::vector<unsigned char> message;
    message.emplace_back(0b11111011);
    pOut->sendMessage(&message);
}

void MusyGen::setPause(bool l) {
    paused_inf = l;
}
