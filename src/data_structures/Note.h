#ifndef MUSYGEN_NOTE_H
#define MUSYGEN_NOTE_H


#include <string>

class Note
{
public:
	int key;
	int velocity;
	double duration;
	double next_note_delay;
	int instrument;

	Note();
	Note(int _key, int _velocity, int _duration, int _next_note_delay,
			int _instrument);
	explicit Note(std::string note_string);

	bool operator==(const Note& other_note) const;

	std::string toString() const;
};


#endif //MUSYGEN_NOTE_H
