#include "../libs/midifile/include/MidiFile.h"
#include "../libs/midifile/include/Options.h"
#include "data_structures/MarkovChain.hpp"
#include "data_structures/VariableMarkovChain.hpp"
#include "data_structures/Note.h"
#include "MusyGen.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "mainwindow.h"
#include <QApplication>

void markovModelTrainingExample(const std::vector<std::string*>& vec)
{
	std::map<std::pair<std::string, std::string>, std::map<std::string, int>> foodmap;
	std::string current = *vec.front();

	// first and last element => two choices

	// take only first and last element
//    if (vec.size() > 2)
//    {
//        foodmap[std::make_pair("", vec.front())][vec[1]] = 1;
//        foodmap[std::make_pair("", vec.back())][vec.front()] = 1;
//    }

	// take first + last and last + first
	if (vec.size() > 2)
	{
		foodmap[std::make_pair(*vec.back(), *vec.front())][*vec[1]] = 1;
		foodmap[std::make_pair(*vec[vec.size() - 2], *vec.back())][*vec.front()] = 1;
	}

	for (int i = 1; i < vec.size(); i++)
	{
		if (i == vec.size() - 1) continue;

		if (foodmap[std::make_pair(current, *vec[i])].find(*vec[i + 1]) !=
			foodmap[std::make_pair(current, *vec[i])].end())
			foodmap[std::make_pair(current, *vec[i])][*vec[i + 1]]++;
		else
			foodmap[std::make_pair(current, *vec[i])][*vec[i + 1]] = 1;

		current = *vec[i];
	}

	VariableMarkovChain<std::string> new_food_chain(2);

	for (const auto& state : foodmap)
	{
		if (!new_food_chain.stateExists(state.first.second))
			new_food_chain.addState(state.first.second);
	}

	for (const auto& state : foodmap)
	{
		std::cout << state.first.first << ", " << state.first.second << ":" << std::endl;

		int total_frequencies = 0;
		for (const auto& frequency : state.second)
			total_frequencies += frequency.second;

		for (const auto& frequency : state.second)
		{
			double probability = (double)frequency.second / total_frequencies;
			new_food_chain.addState(state.first.second);
			new_food_chain.setTransition(state.first.second, { state.first.first }, frequency.first, probability);
			std::cout << "\t" << frequency.first << ": " << probability * 100 << "%" << std::endl;
		}

		std::cout << std::endl;
	}

	MarkovChain<std::vector<std::string>> second_to_first_order_markov_chain = new_food_chain.toFirstOrder();
	std::vector<std::string*> new_food_vec = new_food_chain.randomWalk(100);

	for (const auto& food_string : new_food_vec)
	{
		std::cout << *food_string << std::endl;
	}
}

int main(int argc, char* argv[])
{
//    QApplication a(argc, argv);
//    Mainwindow w;
//    w.show();
//    return a.exec();

	srand((unsigned)time(nullptr));

//	smf::MidiFile midifile;
//	std::string midi_file_name = "twinkle_twinkle";
//	midifile.read("../input_files/midi/" + midi_file_name + ".mid");
//	midifile.doTimeAnalysis();
//	midifile.linkNotePairs();

	MusyGen sys;
	sys.importMidiFile("twinkle_twinkle");

// 	std::cout << midifile << std::endl;

//	int tracks = midifile.getTrackCount();
//	std::cout << "TPQ: " << midifile.getTicksPerQuarterNote() << std::endl;
//	if (tracks > 1) std::cout << "TRACKS: " << tracks << std::endl;
//	for (int track=0; track<tracks; track++) {
//		if (tracks > 1) std::cout << "\nTrack " << track << std::endl;
//		std::cout << "Tick\tSeconds\tDur\tMessage" << std::endl;
//		for (int event=0; event<midifile[track].size(); event++) {
//			std::cout << std::dec << midifile[track][event].tick;
//			std::cout << '\t' << std::dec << midifile[track][event].seconds;
//			std::cout << '\t';
//			if (midifile[track][event].isNoteOn())
//				std::cout << midifile[track][event].getDurationInSeconds();
//			std::cout << '\t' << std::hex;
//			for (int i=0; i<midifile[track][event].size(); i++)
//				std::cout << (int)midifile[track][event][i] << ' ';
//			std::cout << std::endl;
//		}
//	}

//	Note note1(0, 1, 2, 3, 4, 5, 6, 7);
//	Note note2(50, 1, 2, 3, 4, 5, 6, 7);
//	Note note3(25, 1, 2, 3, 4, 5, 6, 7);
//	Note note4(25, 1, 2, 3, 4, 5, 6, 7);
//
//	MarkovChain<Note> note_chain;
//	note_chain.addState(note1);
//	note_chain.addState(note2);
//	note_chain.addState(note3);
//
//	note_chain.setTransition(note1, note2, 0.5);
//	note_chain.setTransition(note1, note4, 0.5);
//
//	note_chain.setTransition(note2, note1, 0.8);
//	note_chain.setTransition(note2, note3, 0.2);
//
//	std::cout << std::boolalpha << note_chain.isLegal() << std::endl; // should give false
//
//	note_chain.setTransition(note4, note2, 0.1);
//	note_chain.setTransition(note3, note1, 0.85);
//	note_chain.setTransition(note4, note3, 0.05);
//	Note note;
//	note_chain.setTransition(note3, note, 0.75); // Should not happen
//
//	std::cout << std::boolalpha << note_chain.isLegal() << std::endl; // should give true
//
//	Note current_state = note_chain.getRandomState();
//	std::cout << current_state.toString() << std::endl;
//
//	for (int i = 0; i < 99; i++)
//	{
//		current_state = note_chain.getNextState(current_state);
//		std::cout << current_state.toString() << std::endl;
//	}


	MarkovChain<std::string> food_chain;
	food_chain.addState("pizza");
	food_chain.addState("ice cream");
	food_chain.addState("chocolate");

	food_chain.setTransition("pizza", "ice cream", 0.5);
	food_chain.setTransition("pizza", "chocolate", 0.5);

	food_chain.setTransition("ice cream", "pizza", 0.8);
	food_chain.setTransition("ice cream", "chocolate", 0.2);

//	std::cout << std::boolalpha << food_chain.isLegal() << std::endl; // should give false

	food_chain.setTransition("chocolate", "ice cream", 0.50);
	food_chain.setTransition("chocolate", "pizza", 0.1);
	food_chain.setTransition("chocolate", "chocolate", 0.40);
	food_chain.setTransition("chocolate", "hamburger", 0.75); // Should not happen

//	std::cout << std::boolalpha << food_chain.isLegal() << std::endl; // should give true

//	markovModelTrainingExample(food_chain.randomWalk(1000));

	return 0;
}
