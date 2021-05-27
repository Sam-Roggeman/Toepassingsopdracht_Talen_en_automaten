cmake_minimum_required(VERSION 3.5)
add_compile_definitions(__WINDOWS_MM__)

project(MusyGen LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
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

        libs/RTMidi/include/RtMidi.h
        libs/RTMidi/src/RtMidi.cpp


        src/data_structures/MarkovChain.hpp
        src/data_structures/VariableMarkovChain.hpp
        src/data_structures/Note.cpp
        src/data_structures/Note.h
        src/MusyGen.cpp
        src/MusyGen.h

#        src/mainwindow.ui
#        src/mainwindow.h
#        src/mainwindow.cpp
#        ./cmake-build-debug/MusyGen_autogen/include/Stopwatch.h

        )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MusyGen
            ${PROJECT_SOURCES}
            )
else()
    if(ANDROID)
        add_library(MusyGen SHARED
                ${PROJECT_SOURCES}
                )
    else()
        add_executable(MusyGen
                ${PROJECT_SOURCES}
                )
    endif()
endif()

target_link_libraries(MusyGen PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Winmm.lib)