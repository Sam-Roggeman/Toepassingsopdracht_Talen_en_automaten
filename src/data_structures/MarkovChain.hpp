#ifndef MUSYGEN_MARKOVCHAIN_HPP
#define MUSYGEN_MARKOVCHAIN_HPP


#include <vector>
#include <map>
#include <set>
#include <iostream>

template<typename T>

class MarkovChain
{
	struct State
	{
		explicit State(const T& _data)
		{
			data = _data;
		}

		T data;
		std::map<double, std::set<State*>> transitions;
	};

	std::set<State*> states;
	State* start_state = nullptr;

public:
	MarkovChain() = default;

	~MarkovChain()
	{
		for (auto& state : states) delete state;
	}

	void addState(const T& data)
	{
		if (stateExists(data)) return;

		State* new_state = new State(data);
		states.insert(new_state);
	}

	void removeState(const T& data)
	{
		if (!stateExists(data)) return;

		State* state_to_remove = getState(data);
		for (auto& state : states)
		{
			for (auto& state2 : state->transitions)
			{
				if (state2.second.find(state_to_remove) != state2.second.end())
					state->transitions.erase(state2.first);
			}
		}
		states.erase(states.find(state_to_remove));
	}

	void setTransition(const T& data_state_1, const T& data_state_2, double probability)
	{
		State* state_1 = getState(data_state_1);
		State* state_2 = getState(data_state_2);
		if (!state_1 || !state_2) return;

		if (state_1->transitions[probability].empty())
			state_1->transitions[probability] = { state_2 };
		else
			state_1->transitions[probability].insert(state_2);
	}

	const T& getRandomState()
	{
		double chance = 1.0 / states.size();
		double rand_number = ((double)rand() / (RAND_MAX));
		double current_number = 0;

		for (const auto& state : states)
		{
			current_number += chance;
			if (rand_number < current_number) return state->data;
		}
		return (*states.end())->data;
	}

	const T& getNextState(const T& data)
	{
		return getNextState(getState(data))->data;
	}

	std::vector<T*> randomWalk(unsigned int iterations)
	{
		std::vector<T*> result;
		State* current_state;

		double chance = 1.0 / states.size();
		double rand_number = ((double)rand() / (RAND_MAX));
		double current_number = 0;

		// first state
		for (const auto& state : states)
		{
			current_number += chance;
			if (rand_number < current_number) current_state = state;
		}

		result.push_back(&current_state->data);

		for (int i = 0; i < iterations - 1; i++)
		{
			current_state = getNextState(current_state);
			result.push_back(&current_state->data);
		}

		return result;
	}

	bool stateExists(const T& data)
	{
		return getState(data);
	}

	bool isLegal()
	{
		for (const auto& state : states)
		{
			double sum_of_probabilities = 0;
			for (const auto& transition : state->transitions)
			{
				for (const auto& state2 : transition.second)
					sum_of_probabilities += transition.first;
			}
			// floating point rounding correction
			if (std::abs(1 - sum_of_probabilities) > 0.00001) return false;
		}
		return true;
	}

private:
	State* getState(const T& data)
	{
		for (const auto state : states) if (state->data == data) return state;
		return nullptr;
	}

	State* getNextState(State* current_state)
	{
		double rand_number = ((double)rand() / (RAND_MAX));
		double current_number = 0;

		for (const auto& transition : current_state->transitions)
		{
			for (const auto& state2 : transition.second)
			{
				current_number += transition.first;
				if (rand_number < current_number) return state2;
			}
		}
		return *states.end();
	}
};


#endif //MUSYGEN_MARKOVCHAIN_HPP
