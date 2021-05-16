#ifndef MUSYGENTEST_VARIABLEMARKOVCHAIN_HPP
#define MUSYGENTEST_VARIABLEMARKOVCHAIN_HPP


#include <vector>
#include <map>
#include <set>
#include <iostream>

template <typename T>
class VariableMarkovChain
{
    struct State
    {
        State(const T& _data)
        {
            data = _data;
        }

        T data;
        std::map<double, std::set<State*> > transitions;
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
        double randnumber = ((double) rand() / (RAND_MAX));
        double current_number = 0;

        for (auto& state : states)
        {
            current_number += chance;
            if (randnumber < current_number) return state->data;
        }
        return (*states.end())->data;
    }

    const T& getNextState(const T& data)
    {
        State* current_state = getState(data);
        double randnumber = ((double) rand() / (RAND_MAX));
        double current_number = 0;

        for (auto& transition : current_state->transitions)
        {
            for (auto& state2 : transition.second)
            {
                current_number += transition.first;
                if (randnumber < current_number) return state2->data;
            }
        }
        return (*states.end())->data;
    }

    bool stateExists(const T& data)
    {
        return getState(data);
    }
    bool isLegal()
    {
        for (auto& state : states)
        {
            double sum_of_probabilities = 0;
            for (auto& transition : state->transitions)
            {
                for (auto& state2 : transition.second)
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
        for (auto state : states) if (state->data == data) return state;
        return nullptr;
    }
};


#endif //MUSYGENTEST_VARIABLEMARKOVCHAIN_HPP
