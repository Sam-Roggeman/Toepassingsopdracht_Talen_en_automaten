#ifndef MUSYGEN_NOTE_H
#define MUSYGEN_NOTE_H


#include <string>

class Note
{
public:
	int key;
	int start_velocity;
	int end_velocity;
	int duration;
	int next_note_delay;
	int tempo;
	int instrument;
	int time_stamp;

	Note();
	Note(int _key, int _start_velocity, int _end_velocity, int _duration, int _next_note_delay, int _tempo,
			int _instrument, int _time_stamp);
	explicit Note(std::string note_string);

	bool operator==(const Note& other_note) const;

	std::string toString() const;
};


#endif //MUSYGEN_NOTE_H
