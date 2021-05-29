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

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Mainwindow w;
    w.show();
    return a.exec();

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

//	MusyGen sys;
//	sys.setPlayInf(true);
//	sys.importMidiFile("./../midi_files/input_files/Never.mid"); // Super Mario 64 - Dire Dire Docks.mid
//	sys.setMarkovOrder(1);
//	sys.trainMarkovModel();
//	sys.playMusicInfinitely();
//	sys.generateMusic(120);
//	sys.exportMidiFile("./../midi_files/output_files/Never.mid");

//	std::cout << sys.generated_midifile << std::endl;
//    std::cout << sys.input_midifile << std::endl;
//    std::cout << "#############################################################" << std::endl;
//    std::cout << sys.generated_midifile << std::endl;


//	return 0;
}
