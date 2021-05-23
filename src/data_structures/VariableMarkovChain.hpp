#ifndef MUSYGEN_VARIABLEMARKOVCHAIN_HPP
#define MUSYGEN_VARIABLEMARKOVCHAIN_HPP


#include <vector>
#include <map>
#include <set>
#include <iostream>

template<typename T>

class VariableMarkovChain
{
	struct State
	{
		explicit State(const T& _data)
		{
			data = _data;
		}

		T data;
		std::map<std::vector<State*>, std::map<double, std::set<State*>>> transitions;
	};

	std::set<State*> states;
	unsigned int order = 2;
	std::ostream& error_stream = std::cerr;

public:
	VariableMarkovChain() = default;

	explicit VariableMarkovChain(unsigned int _order)
	{
		order = _order;
	}

	~VariableMarkovChain()
	{
		clear();
	}

	void setOrder(unsigned int _order)
	{
		order = _order;
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

	void setTransition(const T& data_state_1, const std::vector<T>& previous_data_state_1, const T& data_state_2,
			double probability)
	{
		State* state_1 = getState(data_state_1);
		State* state_2 = getState(data_state_2);
		if (!state_1 || !state_2) return;

		if (previous_data_state_1.size() + 1 > order)
		{
			error_stream << "Error: VariableMarkovChain previous_data_vec is bigger than the markov_order" << std::endl;
			return;
		}
		std::vector<State*> previous_states;
		for (const auto& previous_data : previous_data_state_1)
		{
			if (!stateExists(previous_data))
			{
				error_stream << "Error: VariableMarkovChain previous_data does not exist in states" << std::endl;
				return;
			}

			previous_states.push_back(getState(previous_data));
		}

		if (state_1->transitions[previous_states][probability].empty())
			state_1->transitions[previous_states][probability] = { state_2 };
		else
			state_1->transitions[previous_states][probability].insert(state_2);
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

	const T& getNextState(const T& data, const std::vector<T>& previous_data_vec)
	{
		if (previous_data_vec.size() + 1 > order)
		{
			error_stream << "Error: VariableMarkovChain previous_data_vec is bigger than the markov_order" << std::endl;
			return (*states.end())->data;
		}

		std::vector<State*> previous_states;
		for (const auto& previous_data : previous_data_vec)
		{
			if (!stateExists(previous_data))
			{
				error_stream << "Error: VariableMarkovChain previous_data does not exist in states" << std::endl;
				return (*states.end())->data;
			}

			previous_states.push_back(getState(previous_data));
		}

		return getNextState(getState(data), previous_states)->data;
	}

	std::vector<T*> randomWalk(unsigned int iterations)
	{
		std::vector<T*> result;
		State* current_state;
		std::vector<State*> previous_states;

		double chance = 1.0 / states.size();
		double rand_number = ((double)rand() / (RAND_MAX));
		double current_number = 0;

		// first state
		for (const auto& state : states)
		{
			current_number += chance;
			if (rand_number < current_number) current_state = state;
		}

		// chooses random previous states set from first state
		chance = 1.0 / current_state->transitions.size();
		rand_number = ((double)rand() / (RAND_MAX));
		current_number = 0;

		for (const auto& previous_states_set : current_state->transitions)
		{
			current_number += chance;
			if (rand_number < current_number) previous_states = previous_states_set.first;
		}

		result.push_back(&current_state->data);

		for (int i = 0; i < iterations - 1; i++)
		{
			State* next_state = getNextState(current_state, previous_states);
			result.push_back(&next_state->data);
			previous_states.erase(previous_states.begin());
			previous_states.push_back(current_state);
			current_state = next_state;
		}

		return result;
	}

	MarkovChain<std::vector<T>> toFirstOrder()
	{
		MarkovChain<std::vector<T>> first_order_markov_chain;

		// states
		for (const auto& state : states)
		{
			for (const auto& previous_states_set : state->transitions)
			{
				std::vector<T> new_data;

				for (const auto& previous_state : previous_states_set.first)
					new_data.push_back(previous_state->data);

				new_data.push_back(state->data);
				first_order_markov_chain.addState(new_data);
			}
		}

		// transitions
		for (const auto& state : states)
		{
			for (const auto& previous_states_set : state->transitions)
			{
				std::vector<T> new_data;

				for (const auto& previous_state : previous_states_set.first)
					new_data.push_back(previous_state->data);

				new_data.push_back(state->data);

				for (const auto& transition : previous_states_set.second)
				{
					for (const auto& state2 : transition.second)
					{
						std::vector<T> new_data2 = new_data;
						new_data2.erase(new_data2.begin());
						new_data2.push_back(state2->data);
						first_order_markov_chain.setTransition(new_data, new_data2, transition.first);
					}
				}
			}
		}

		return first_order_markov_chain;
	}

	std::vector<T> getAllStates() const
	{
		std::vector<T> all_data;

		for (const auto& state : states)
			all_data.push_back(state->data);

		return all_data;
	}

	[[nodiscard]] bool stateExists(const T& data) const
	{
		for (const auto state : states) if (state->data == data) return true;
		return false;
	}

	bool isLegal()
	{
		for (auto& state : states)
		{
			double sum_of_probabilities = 0;
			for (const auto& previous_data : state->transitions)
			{
				if (previous_data.first.size() + 1 > order)
				{
					error_stream << "Error: VariableMarkovChain previous_data_vec is bigger than the markov_order"
								 << std::endl;
					return false;
				}

				for (const auto& transition : previous_data.second)
				{
					for (const auto& state2 : transition.second)
						sum_of_probabilities += transition.first;
				}
			}
			// floating point rounding correction
			if (std::abs(1 - sum_of_probabilities) > 0.00001) return false;
		}
		return true;
	}

	[[nodiscard]] bool empty() const
	{
		return states.empty();
	}

	void clear()
	{
		for (auto& state : states) delete state;
	}

	// todo: implement operator= overloader

private:
	State* getState(const T& data)
	{
		for (const auto state : states) if (state->data == data) return state;
		return nullptr;
	}

	State* getNextState(State* current_state, std::vector<State*> previous_states_vec)
	{
		double random_number = ((double)rand() / (RAND_MAX));
		double current_number = 0;

		for (const auto& transition : current_state->transitions[previous_states_vec])
		{
			for (const auto& state2 : transition.second)
			{
				current_number += transition.first;
				if (random_number < current_number) return state2;
			}
		}
		return *states.end();
	}
};


#endif //MUSYGEN_VARIABLEMARKOVCHAIN_HPP
