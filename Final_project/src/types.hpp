#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <map>
#include <climits>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/mw_matching.h>

struct Transition;
struct State;

struct Transition {
    std::string output;
    double prob;
    State* next;

    Transition(std::string output, State* next, double prob) {
        this->output = output;
        this->next = next;
        this->prob = prob;
    }
};

struct State {
    std::string name;
    std::string encoded;
    int whichGroup;
    double prob;
    std::map<std::string, Transition*> transitions;

    State(std::string name){
        this->name = name;
    }
};

struct Info {
    std::string encoded;
    std::string neighbor;

    Info() {}
};

#endif // TYPES_HPP