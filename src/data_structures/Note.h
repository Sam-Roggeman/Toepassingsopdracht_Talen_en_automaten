#ifndef MUSYGEN_NOTE_H
#define MUSYGEN_NOTE_H

#include <tuple>
#include <string>
#include <vector>

class Note
{
public:
	int key;
	int velocity;
	int duration;
	int next_note_delay;
	int instrument;
	int track;
	int channel;
	int tempo;
	std::vector<std::pair<int, int>> controllers;

	Note();
	Note(int _key, int _velocity, int _duration, int _next_note_delay,
			int _instrument, int _track, int _channel, int _tempo, const std::vector<std::pair<int,int>>& _controllers);
	explicit Note(std::string note_string);

	bool operator==(const Note& other_note) const;
	bool operator<(const Note& other_note) const;
	bool operator<=(const Note& other_note) const;
	bool operator>(const Note& other_note) const;
	bool operator>=(const Note& other_note) const;

	[[nodiscard]] std::string toString() const;
};


#endif //MUSYGEN_NOTE_H
