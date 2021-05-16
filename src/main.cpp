#include "../libs/midifile/include/MidiFile.h"
#include "../libs/midifile/include/Options.h"
#include "data_structures/MarkovChain.hpp"
#include "data_structures/Note.h"
#include "MusyGen.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "mainwindow.h"
#include <QApplication>

void calculateMarkovThing(const std::vector<std::string>& vec)
{
    std::map<std::pair<std::string, std::string>, std::map<std::string, int>> foodmap;
    std::string current = vec.front();
    for (int i = 1; i < vec.size(); i++)
    {
        if (i == vec.size() - 1) continue;

//        std::cout << current + " " + vec[i] << std::endl;
        if (foodmap[std::make_pair(current, vec[i])].find(vec[i + 1]) != foodmap[std::make_pair(current, vec[i])].end())
            foodmap[std::make_pair(current, vec[i])][vec[i + 1]]++;
        else
            foodmap[std::make_pair(current, vec[i])][vec[i + 1]] = 1;

        current = vec[i];
    }

    for (const auto& state : foodmap)
    {
        std::cout << state.first.first << ", " << state.first.second << ":" << std::endl;
        for (const auto& frequency : state.second)
        {
            std::cout << "\t" << frequency.first << ": " << frequency.second << std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    Mainwindow w;
//    w.show();
//    return a.exec();

//	smf::MidiFile midifile;
//	std::string midi_file_name = "Super Mario 64 - Dire Dire Docks";
//	midifile.read("../input_files/midi/" + midi_file_name + ".mid");
//	midifile.doTimeAnalysis();
//	midifile.linkNotePairs();



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

	srand( (unsigned)time(nullptr) );

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

    std::vector<std::string> foods;

	std::string current_state = food_chain.getRandomState();
//	std::cout << current_state << " ";
    foods.push_back(current_state);

	for (int i = 0; i < 99; i++)
	{
		current_state = food_chain.getNextState(current_state);
//		std::cout << current_state << " ";
        foods.push_back(current_state);
	}

	calculateMarkovThing(foods);

//    MusyGen sys;
//    sys.loadMidi();

// pizza chocolate ice cream pizza chocolate ice cream pizza chocolate ice cream pizza

/**
 * pizza
 * pizza chocolate
 * chocolate ice cream
 * ice cream pizza
 * pizza chocolate
 * chocolate ice cream
 * chocolate ice cream
 * ice cream pizza
 * pizza chocolate
 * chocolate ice cream
 * ice cream pizza
 *
 * pizza
 * pizza chocolate
 *  - ice cream 3 => kans = 1
 *  - chocolate 0
 *  - pizza 0
 * chocolate ice cream
 * ice cream pizza
 */

	return 0;
}
