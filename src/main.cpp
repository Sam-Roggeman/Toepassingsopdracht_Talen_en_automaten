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

//	smf::MidiFile midifile;
//	std::string midi_file_name = "twinkle_twinkle";
//	midifile.read("../input_files/midi/" + midi_file_name + ".mid");
//	midifile.doTimeAnalysis();
//	midifile.linkNotePairs();
//
//	std::ofstream outputfile("twinkle_twinkle_export2.mid");
//	midifile.write(outputfile);
//	outputfile.close();

// 	std::cout << midifile << std::endl;
//
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

	MusyGen sys;
	sys.importMidiFile("plastic_love");
//	sys.trainMarkovModel();
//	sys.generateMusic(120);

//	std::cout << sys.generated_midifile << std::endl;
//    std::cout << sys.input_midifile << std::endl;
//    std::cout << "#############################################################" << std::endl;
//    std::cout << sys.generated_midifile << std::endl;
	sys.exportInputMidiFile("plastic_love");

	return 0;
}
