cmake_minimum_required(VERSION 3.17)
project(MusyGenTest)

set(CMAKE_CXX_STANDARD 14)

add_executable(MusyGenTest
        src/main.cpp

        libs/midifile/include/Binasc.h
        libs/midifile/include/MidiEvent.h
        libs/midifile/include/MidiEventList.h
        libs/midifile/include/MidiFile.h
        libs/midifile/include/MidiMessage.h
        libs/midifile/include/Options.h

        libs/midifile/src/Binasc.cpp
        libs/midifile/src/MidiEvent.cpp
        libs/midifile/src/MidiEventList.cpp
        libs/midifile/src/MidiFile.cpp
        libs/midifile/src/MidiMessage.cpp
        libs/midifile/src/Options.cpp

        src/data_structures/MarkovChain.hpp
        src/data_structures/VariableMarkovChain.hpp
        src/data_structures/Note.cpp
        src/data_structures/Note.h
        src/MusyGen.cpp
        src/MusyGen.h)