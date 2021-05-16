#include "Note.h"

Note::Note()
{
	key = 0;
	start_velocity = 0;
	end_velocity = 0;
	duration = 0;
	next_note_delay = 0;
	tempo = 0;
	instrument = 0;
	time_stamp = 0;
}

Note::Note(int _key, int _start_velocity, int _end_velocity, int _duration, int _next_note_delay, int _tempo,
		int _instrument, int _time_stamp)
{
	key = _key;
	start_velocity = _start_velocity;
	end_velocity = _end_velocity;
	duration = _duration;
	next_note_delay = _next_note_delay;
	tempo = _tempo;
	instrument = _instrument;
	time_stamp = _time_stamp;
}

Note::Note(std::string note_string)
{
	std::string sub_string;
	std::string sub_string2;
	size_t pos;

	while (!note_string.empty())
	{
		pos = note_string.find(':');
		sub_string = note_string.substr(0, pos);
		note_string.erase(0, pos + 2);

		pos = note_string.find(',');
		if (pos != std::string::npos)
		{
			sub_string2 = note_string.substr(0, pos);
			note_string.erase(0, pos + 2);
		}
		else
		{
			sub_string2 = note_string;
			note_string.clear();
		}

		if (sub_string == "key")
			key = std::stoi(sub_string2);
		else if (sub_string == "start_velocity")
			start_velocity = std::stoi(sub_string2);
		else if (sub_string == "end_velocity")
			end_velocity = std::stoi(sub_string2);
		else if (sub_string == "duration")
			duration = std::stoi(sub_string2);
		else if (sub_string == "next_note_delay")
			next_note_delay = std::stoi(sub_string2);
		else if (sub_string == "tempo")
			tempo = std::stoi(sub_string2);
		else if (sub_string == "instrument")
			instrument = std::stoi(sub_string2);
		else if (sub_string == "time_stamp")
			time_stamp = std::stoi(sub_string2);
	}
}

std::string Note::toString() const
{
	std::string note_string;

	note_string += "key: " + std::to_string(key);
	note_string += ", start_velocity: " + std::to_string(start_velocity);
	note_string += ", end_velocity: " + std::to_string(end_velocity);
	note_string += ", duration: " + std::to_string(duration);
	note_string += ", next_note_delay: " + std::to_string(next_note_delay);
	note_string += ", tempo: " + std::to_string(tempo);
	note_string += ", instrument: " + std::to_string(instrument);
	note_string += ", time_stamp: " + std::to_string(time_stamp);

	return note_string;
}

bool Note::operator==(const Note& other_note) const
{
	return (key == other_note.key
			&& start_velocity == other_note.start_velocity
			&& end_velocity == other_note.end_velocity
			&& duration == other_note.duration
			&& next_note_delay == other_note.next_note_delay
			&& tempo == other_note.tempo
			&& instrument == other_note.instrument
			&& time_stamp == other_note.time_stamp);
}