#include "MusyGen.h"


void MusyGen::importMidiFile(const std::string& filename)
{
	std::vector<std::vector<Note>> notes;

    input_midifile.read("../input_files/midi/" + filename + ".mid");
    input_midifile.doTimeAnalysis();
    input_midifile.linkNotePairs();

    int tracks = input_midifile.getTrackCount();
    for (int track=1; track < tracks; track++)
    {
        for (int event = 0; event < input_midifile[track].size(); event++)
        {

            if (input_midifile[track][event].isNoteOn())
            {
                std::cout << "NoteOn:  " << input_midifile[track][event].getKeyNumber() << ", " << input_midifile[track][event].getDurationInSeconds() <<  std::endl;
                Note(input_midifile[track][event].getKeyNumber(),input_midifile[track][event].getVelocity(),input_midifile[track][event].getDurationInSeconds(),input_midifile[track][event]);
            }
            if (input_midifile[track][event].isNoteOff())
            {
               std::cout << "NoteOff: " << input_midifile[track][event].getKeyNumber() << ", " << input_midifile[track][event].getDurationInSeconds() <<  std::endl;
            }

    /*
    * int tracks = input_midifile.getTrackCount();
//    std::cout << "TPQ: " << input_midifile.getTicksPerQuarterNote() << std::endl;
    if (tracks > 1) std::cout << "TRACKS: " << tracks << std::endl;
    for (int track=1; track<tracks; track++) {
        if (tracks > 1) std::cout << "\nTrack " << track << std::endl;
//        std::cout << "Tick\tSeconds\tDur\tMessage" << std::endl;
        for (int event=0; event<input_midifile[track].size()-1; event++) {
//            std::cout << std::dec << input_midifile[track][event].tick;
//            std::cout << '\t' << std::dec << input_midifile[track][event].seconds;
//            std::cout << '\t';
            if (input_midifile[track][event].isNoteOn())
                //std::cout << input_midifile[track][event].getDurationInSeconds();
            std::cout << '\t' << std::hex;
            for (int i=0; i<input_midifile[track][event].size(); i++)
            {
//                std::cout << (int)input_midifile[track][event][i] << ' ';
                if (input_midifile[track][event].isController())
                    continue;

                std::cout << input_midifile[track][event].getKeyNumber() << std::endl;
                input_midifile[track][event].getKeyNumber();
            }
            std::cout << std::endl;
        }
    }*/
	trainMarkovModel(notes);
        }
    }
}

void MusyGen::exportMidiFile(const std::string& filename)
{

}

void MusyGen::generateMusic(unsigned int duration)
{

}

void MusyGen::playMusicInfinitly()
{

    bool playing = true;
    while (playing){

    }
}

void MusyGen::trainMarkovModel(const std::vector<std::vector<Note>>& notes)
{
	if (order == 1)
	{

	}
	else
	{

	}
}
