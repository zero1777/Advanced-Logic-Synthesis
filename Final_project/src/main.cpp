#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include "types.hpp"
#include "../glpk-4.47/src/glpk.h"

using namespace leda;

std::map<std::string, State*> states;
std::map<std::string, int> weights;
std::map<std::string, double> cost_weights;
std::string reset_state;
int num_inputs, num_outputs, num_transitions, num_states;
int n, totalGroup;

void parse(std::string filename);
void solve_probability();
void initial_encoding();
void optimizing_cost_function(double e, double r);
void output_blif(std::string filename, std::string output_filename);
std::map<std::string, Info> perturb(std::map<std::string, Info> &state_info, std::vector<std::string> &state_names, int type);
double calculateCostFunc(std::map<std::string, Info> &state_info);
int calculateDistance(std::string s1, std::string s2);
double calculateWeight(std::string s1, std::string s2);


// int main()
// {
//   graph G; 

//   node n0=G.new_node(); node n1=G.new_node();
//   node n2=G.new_node(); node n3=G.new_node();
//   node n4=G.new_node(); node n5=G.new_node();
//   node n6=G.new_node();
    
//   edge e0=G.new_edge(n0,n1); edge e1=G.new_edge(n0,n2);
//   edge e2=G.new_edge(n0,n3); edge e3=G.new_edge(n0,n4);
//   edge e4=G.new_edge(n0,n5); edge e5=G.new_edge(n0,n6);
//     edge e6=G.new_edge(n1,n2); edge e7=G.new_edge(n1,n3);
//     edge e8=G.new_edge(n1,n4); edge e9=G.new_edge(n1,n5);
//     edge e10=G.new_edge(n1,n6); edge e11=G.new_edge(n2,n3);
//     edge e12=G.new_edge(n2,n4); edge e13=G.new_edge(n2,n5);
//     edge e14=G.new_edge(n2,n6); edge e15=G.new_edge(n3,n4);
//     edge e16=G.new_edge(n3,n5); edge e17=G.new_edge(n3,n6);
//     edge e18=G.new_edge(n4,n5); edge e19=G.new_edge(n4,n6);
//     edge e20=G.new_edge(n5,n6);

//   weight[e0]=4;  weight[e1]=3;  weight[e2]=1;
//   weight[e3]=4;  weight[e4]=5; weight[e5]=2;
//   weight[e6]=4; weight[e7]=2; weight[e8]=8;
//   weight[e9]=4; weight[e10]=6; weight[e11]=1;
//     weight[e12]=4; weight[e13]=0; weight[e14]=3;
//     weight[e15]=4; weight[e16]=4; weight[e17]=5;
//     weight[e18]=2; weight[e19]=1; weight[e20]=9;
 
//   list<edge> M=MAX_WEIGHT_MATCHING(G,weight);

//   std::cout << "Maximum Weighted Matching:" << std::endl;
//   int weight_M=0;
//   edge edge_result;
//   forall(edge_result,M) {G.print_edge(edge_result); weight_M+=weight[e2];}
//   std::cout << " weight: " << weight_M << std::endl;

//   return 0;
// }


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Need the testcase argument!" << std::endl;
        return 1;
    }
    std::string testcase_name(argv[1]);
    std::string filename = "../benchmarks/" + testcase_name + ".kiss";
    std::string init_output_filename = "../results/" + testcase_name + "_init.blif";
    std::string output_filename = "../results/" + testcase_name + ".blif";
    // std::cout << "filename: " << filename << std::endl;
    // std::cout << "init filename: " << init_output_filename << std::endl;
    // std::cout << "output filename: " << output_filename << std::endl;
    // std::string filename = "../benchmarks/cse_opt.kiss";
    // std::string init_output_filename = "../results/cse_opt_init.blif";
    // std::string output_filename = "../results/cse_opt.blif";
    srand(time(NULL));
    parse(filename);
    solve_probability();
    initial_encoding();
    // output_blif(filename, init_output_filename);
    optimizing_cost_function(0.1, 0.85);
    output_blif(filename, output_filename);
    return 0;
}

void parse(std::string filename) {
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line); // .start_kiss
    std::cout << "Start kiss: " << line << std::endl;
    while (std::getline(file, line)) { // .i, .o, .p, .s, .r
        std::stringstream ss(line);
        std::string token;
        ss >> token;
        if (token == ".i") {
            ss >> num_inputs;
            std::cout << "Number of inputs: " << num_inputs << std::endl;
        } else if (token == ".o") {
            ss >> num_outputs;
            std::cout << "Number of outputs: " << num_outputs << std::endl;
        } else if (token == ".p") {
            ss >> num_transitions;
            std::cout << "Number of transitions: " << num_transitions << std::endl;
        } else if (token == ".s") {
            ss >> num_states;
            std::cout << "Number of states: " << num_states << std::endl;
            n = (int) std::ceil(std::log2(num_states));
            std::cout << "Number of state bits: " << n << std::endl;
        } else if (token == ".r") {
            ss >> reset_state;
            std::cout << "Reset state: " << reset_state << std::endl;
            break;
        } else {
            std::cout << "Unknown token: " << token << std::endl;
        }
    }

    for (int i = 0; i < num_transitions; i++) {
        std::getline(file, line);
        std::stringstream ss(line);
        std::string cur_state, next_state, input, output;
        ss >> input >> cur_state >> next_state >> output;
        // create states if they don't exist
        if (states.find(cur_state) == states.end()) {
            // std::cout << "New state: " << cur_state << std::endl;
            states[cur_state] = new State(cur_state);
        }
        if (states.find(next_state) == states.end()) {
            // std::cout << "New state: " << next_state << std::endl;
            states[next_state] = new State(next_state);
        }
        // calculate transition probability
        double prob = 1.0;
        for (int k = 0; k < input.length(); k++) {
            if (input[k] == '0' || input[k] == '1') {
                prob *= 0.5;
            }
        }
        // create transition
        states[cur_state]->transitions[input] = new Transition(output, states[next_state], prob);
        // std::cout << "Transition " << i << ": " << input << " " << output << " " << cur_state << "->" << next_state << " " << prob << std::endl;
    }

    std::getline(file, line); // .end_kiss
    std::cout << "End kiss: " << line << std::endl;
}

void output_blif(std::string filename, std::string output_filename) {
    std::ifstream infile(filename);
    std::ofstream outfile(output_filename);
    std::string line;

    outfile << ".model myfsm" << std::endl;
    while (std::getline(infile, line)) {
        outfile << line << std::endl;
    }
    for (auto it = states.begin(); it != states.end(); it++) {
        State *state = it->second;
        outfile << ".code " << state->name << " " << state->encoded << std::endl;
        std::cout << ".code " << state->name << " " << state->encoded << std::endl;
    }
    outfile << ".end" << std::endl;
    outfile.close();
}

void solve_probability() {
    glp_prob *lp;
    int ia[1001], ja[1001];
    double ar[1001];
    lp = glp_create_prob();
    glp_set_prob_name(lp, "state probability model");
    glp_set_obj_dir(lp, GLP_MIN);
    // add constraints (rows)
    glp_add_rows(lp, num_states + 1);
    glp_set_row_name(lp, num_states + 1, "total");
    glp_set_row_bnds(lp, num_states + 1, GLP_FX, 1.0, 1.0);
    for (int i = 1; i <= num_states; i++) {
        std::string name = "ab" + std::to_string(i);
        glp_set_row_name(lp, i, name.c_str());
        glp_set_row_bnds(lp, i, GLP_FX, 0.0, 0.0);
    }
    // add variables (columns)
    glp_add_cols(lp, num_states);
    std::vector<std::string> state_names(num_states+1);
    std::map<std::string, int> state_indices;
    int col = 1;
    for (auto it = states.begin(); it != states.end(); it++) {
        std::string name = it->first;
        glp_set_col_name(lp, col, name.c_str());
        glp_set_col_bnds(lp, col, GLP_DB, 0.0, 1.0);
        state_indices[name] = col;
        state_names[col++] = name;
    }
    // initialize matrix
    for (int i = 1; i <= num_states + 1; i++) {
        for (int j = 1; j <= num_states; j++) {
            int idx = (i - 1) * num_states + j;
            ia[idx] = i;
            ja[idx] = j;
            if (i == num_states + 1) {
                ar[idx] = 1.0;
            } else {
                if (i == j) {
                    ar[idx] = -1.0;
                } else {
                    ar[idx] = 0.0;
                }
            }
        }
    }
    // add coefficients
    for (auto it = states.begin(); it != states.end(); it++) {
        std::string name = it->first;
        State *state = it->second;
        for (auto it2 = state->transitions.begin(); it2 != state->transitions.end(); it2++) {
            Transition *transition = it2->second;
            int next_state_row = state_indices[transition->next->name];
            int idx = (next_state_row - 1) * num_states + state_indices[name];
            ar[idx] += transition->prob;
            // std::cout << "current state: " << name << " next state: " << transition->next->name << " prob: " << transition->prob << std::endl;
            // std::cout << "ia: " << ia[idx] << " ja: " << ja[idx] << " ar: " << ar[idx] << std::endl;
        }
    } 
    // solve the LP
    glp_load_matrix(lp, num_states * (num_states + 1), ia, ja, ar);
    glp_simplex(lp, NULL);
    double z = glp_get_obj_val(lp);
    for (int i = 1; i <= num_states; i++) {
        double x = glp_get_col_prim(lp, i);
        states[state_names[i]]->prob = x;
        std::cout << state_names[i] << ": " << x << std::endl;
    }
    glp_delete_prob(lp);
}

// /*
void initial_encoding() {
    graph G;
    G.make_undirected();
    node v[num_states];
    edge e[num_states * (num_states - 1) / 2];
    std::map<node, int> node_indices;
    std::map<std::string, int> edge_indices;
    std::vector<std::string> state_names;
    int edge_index = 0;

    for (auto it = states.begin(); it != states.end(); it++) {
        std::string name = it->first;
        state_names.push_back(name);
    }
    for (int i = 0; i < num_states; i++) {
        v[i] = G.new_node();
        node_indices[v[i]] = i;
    }
    for (int i = 0; i < num_states; i++) {
        for (int j = i + 1; j < num_states; j++) {
            std::string name = states[state_names[i]]->name + "_" + states[state_names[j]]->name;
            edge_indices[name] = edge_index;
            e[edge_index++] = G.new_edge(v[i], v[j]);
        }
    }

    edge_array<int> weight(G);
    for (int i = 0; i < num_states * (num_states - 1) / 2; i++) {
        weight[e[i]] = 0;
    }

    // transition to the same next state
    std::map<std::string, std::set<std::string>> next_states;
    for (auto it = states.begin(); it != states.end(); it++) {
        std::string name = it->first;
        State *state = it->second;
        for (auto it2 = state->transitions.begin(); it2 != state->transitions.end(); it2++) {
            Transition *transition = it2->second;
            next_states[transition->next->name].insert(name);
        }
    }
    for (auto it = next_states.begin(); it != next_states.end(); it++) {
        std::string next_state = it->first;
        std::set<std::string> prev_states = it->second;
        for (auto s : prev_states) {
            for (auto s2 : prev_states) {
                std::string name = s + "_" + s2;
                if (edge_indices.find(name) != edge_indices.end()) {
                    weight[e[edge_indices[name]]] += n;
                }
            }
        }
        // std::cout << "next_state: " << next_state << std::endl;
        // std::cout << "states: ";
        // for (auto s : states) {
        //     std::cout << s << " ";
        // }
        // std::cout << std::endl; 
    }

    // assert the same output
    std::map<std::string, std::set<std::string>> output_to_states;
    for (auto it = states.begin(); it != states.end(); it++) {
        std::string name = it->first;
        State *state = it->second;
        // std::cout << "state: " << name << std::endl;
        for (auto it2 = state->transitions.begin(); it2 != state->transitions.end(); it2++) {
            Transition *transition = it2->second;
            // std::cout << "transition: " << transition->next->name << " " << transition->output << std::endl;
            output_to_states[transition->output].insert(transition->next->name);
        }
    }
    for (auto it = output_to_states.begin(); it != output_to_states.end(); it++) {
        std::string output = it->first;
        std::set<std::string> record_states = it->second;
        if (record_states.size() <= 1) continue;
        for (auto s : record_states) {
            for (auto s2 : record_states) {
                std::string name = s + "_" + s2;
                if (edge_indices.find(name) != edge_indices.end()) {
                    weight[e[edge_indices[name]]] += 1;
                }
            }
        }
        // std::cout << "output: " << output << std::endl;
        // std::cout << "states: ";
        // for (auto s : states) {
        //     std::cout << s << " ";
        // }
        // std::cout << std::endl;
    }
    

    // // same state to next state pair
    for (auto it = states.begin(); it != states.end(); it++) {
        std::string name = it->first;
        State *state = it->second;
        std::set<std::string> neighbors;
        // std::cout << "state: " << name << std::endl;
        // std::cout << "neighbors: ";
        for (auto it2 = state->transitions.begin(); it2 != state->transitions.end(); it2++) {
            Transition *transition = it2->second;
            neighbors.insert(transition->next->name);
        }
        for (auto s : neighbors) {
            for (auto s2 : neighbors) {
                std::string name = s + "_" + s2;
                if (edge_indices.find(name) != edge_indices.end()) {
                    weight[e[edge_indices[name]]] += (n / neighbors.size());
                    // weight[e[edge_indices[name]]] += (n / 2);
                }
            }
        }
        // for (auto s : neighbors) {
        //     std::cout << s << " ";
        // }
        // std::cout << std::endl;
    }

    // same input to next state pair
    std::map<std::string, std::set<std::string>> input_to_states;
    for (auto it = states.begin(); it != states.end(); it++) {
        std::string name = it->first;
        State *state = it->second;
        // std::cout << "state: " << name << std::endl;
        for (auto it2 = state->transitions.begin(); it2 != state->transitions.end(); it2++) {
            std::string input = it2->first;
            // std::cout << "transition input: " << input << std::endl;
            input_to_states[input].insert(state->name);
        }
    }
    for (auto it = input_to_states.begin(); it != input_to_states.end(); it++) {
        std::string input = it->first;
        std::set<std::string> record_states = it->second;
        if (record_states.size() <= 1) continue;
        for (auto s : record_states) {
            for (auto s2 : record_states) {
                std::string name = s + "_" + s2;
                if (edge_indices.find(name) != edge_indices.end()) {
                    weight[e[edge_indices[name]]] += 1;
                }
            }
        }
        // std::cout << "input: " << input << std::endl;
        // std::cout << "states: ";
        // for (auto s : states) {
        //     std::cout << s << " ";
        // }
        // std::cout << std::endl;
    }

    for (auto it = edge_indices.begin(); it != edge_indices.end(); it++) {
        std::string edge_name = it->first;
        int index = it->second;
        weights[edge_name] = weight[e[index]];
        // std::cout << "edge: " << edge_name << " weight: " << weight[e[index]] << std::endl;
    }

    // weight adjustment
    int min_weight = INT_MAX;
    int max_weight = -1;
    int total_weight = 0;
    for (int i = 0; i < num_states * (num_states - 1) / 2; i++) {
        min_weight = std::min(min_weight, weight[e[i]]);
        max_weight = std::max(max_weight, weight[e[i]]);
        total_weight += weight[e[i]];
    }
    int average_weight = total_weight / (num_states * (num_states - 1) / 2);
    int quarter_weight = (min_weight + average_weight) / 2;
    int three_quarter_weight = (max_weight + average_weight) / 2;
    for (int i = 0; i < num_states * (num_states - 1) / 2; i++) {
        if (weight[e[i]] <= quarter_weight) {
            weight[e[i]] = 0;
        } else if (weight[e[i]] <= average_weight) {
            weight[e[i]] = 0;
        } else if (weight[e[i]] <= three_quarter_weight) {
            // weight[e[i]] = 0;
            weight[e[i]] -= average_weight;
            // weight[e[i]] *= 0.8;
            if (num_states >= 20) weight[e[i]] += i;
        } else {
            weight[e[i]] -= average_weight;
            // weight[e[i]] *= 0.9;    
            if (num_states <= 25) weight[e[i]] += i;
        }
    }
    
    // weight[e[18]] = 0;
    // weight[e[15]] = 0;
    // weight[e[16]] = 0;
    // weight[e[17]] = 0;
    // weight[e[19]] = 0;
    // weight[e[20]] = 0;

    list<edge> M = MAX_WEIGHT_MATCHING(G, weight);
    // list<edge> M = MAX_WEIGHT_PERFECT_MATCHING(G, weight);

    std::vector<State*> state_list;
    std::vector<bool> visited(num_states, false);
    std::cout << "Maximum weight matching finished" << std::endl;
    int weight_M = 0;
    edge e2;
    forall (e2, M) {
        // std::cout << "node: " << state_names[node_indices[G.source(e2)]] << " " << state_names[node_indices[G.target(e2)]] << std::endl;
        G.print_edge(e2);
        std::cout << std::endl;
        // G.print_node(G.source(e2));
        // G.print_node(G.target(e2));
        // weight_M += weight[e2];
        state_list.push_back(states[state_names[node_indices[G.source(e2)]]]);
        state_list.push_back(states[state_names[node_indices[G.target(e2)]]]);
        visited[node_indices[G.source(e2)]] = true;
        visited[node_indices[G.target(e2)]] = true;
    }

    for (int i = 0; i < num_states; i++) {
        if (!visited[i]) {
            // std::cout << "unvisited: " << state_names[i] << std::endl;
            state_list.push_back(states[state_names[i]]);
        }
    }
    // std::cout << "state list: ";
    // for (auto s : state_list) {
    //     std::cout << s->name << " ";
    // }
    // std::cout << std::endl;

    // initial group
    int groupIndex = 0;
    for (int i = 0; i < state_list.size(); i += 2) {
        state_list[i]->whichGroup = groupIndex;
        if (i + 1 < state_list.size()) state_list[i + 1]->whichGroup = groupIndex;
        groupIndex++;
    }
    totalGroup = groupIndex;

    // initial encoding
    int group = 1;
    int bit = 0;
    int cnt = 0;
    while (group < num_states) {
        for (auto s : state_list) {
            s->encoded.push_back('0' + bit);
            cnt++;
            if (cnt == group) {
                cnt = 0;
                bit ^= 1;
            }
        }
        group *= 2;
        cnt = 0;
        bit = 0;
    }

    for (auto s : state_list) {
        std::reverse(s->encoded.begin(), s->encoded.end());
        // std::cout << "state: " << s->name << " encoded: ";
        // for (auto e : s->encoded) {
        //     std::cout << e << " ";
        // }
        // std::cout << std::endl;
    }
}
// */

// /*
void optimizing_cost_function(double e, double r) {
    std::vector<std::vector<std::string>> state_groups(totalGroup);
    std::vector<std::string> state_names;
    double curCost, bestCost;
    int MT, uphill, reject, N;
    double T;
    std::map<std::string, Info> best_state_encoding;
    std::map<std::string, Info> cur_state_encoding;

    // initial state group
    for (auto s : states) {
        State* state = s.second;
        state_groups[state->whichGroup].push_back(state->name);
    }

    // initial cost weight
    for (auto s : states) {
        State* state = s.second;
        for (auto t : state->transitions) {
            Transition* transition = t.second;
            std::string cost_weight_name = state->name + "_" + transition->next->name;
            double _prob = state->prob * transition->prob;
            // std::cout << "state prob: " << state->prob << " transition prob: " << transition->prob << "result prob: " << _prob << std::endl;
            cost_weights[cost_weight_name] += _prob;
            // std::cout << "cost weight: " << cost_weights[cost_weight_name] << std::endl;
        }
    }
    for (auto it = cost_weights.begin(); it != cost_weights.end(); it++) {
        it->second *= 10;
        // std::cout << "cost weight: " << it->first << " " << it->second << std::endl;
    }

    // state names
    for (auto s : states) {
        State *state = s.second;
        state_names.push_back(state->name);
    }

    // initial state encoding
    for (auto s : states) {
        State* state = s.second;
        cur_state_encoding[state->name].encoded = state->encoded;
    }
    for (int i = 0; i < totalGroup; i++) {
        if (state_groups[i].size() == 1) {
            cur_state_encoding[state_groups[i][0]].neighbor = state_groups[i][0];
        } else {
            cur_state_encoding[state_groups[i][0]].neighbor = state_groups[i][1];
            cur_state_encoding[state_groups[i][1]].neighbor = state_groups[i][0];
        }
    }

    // std::cout << "initial state encoding" << std::endl;
    // for (auto it = cur_state_encoding.begin(); it != cur_state_encoding.end(); it++) {
    //     std::cout << it->first << " " << it->second.neighbor << " " << it->second.encoded << std::endl;
    // }
    // std::cout << std::endl;

    best_state_encoding = cur_state_encoding;
    curCost = bestCost = calculateCostFunc(best_state_encoding);
    // std::cout << "initial cost: " << curCost << std::endl;

    MT = uphill = reject = 0;
    T = 1000;
    N = num_states * num_states / 2; // TODO: change N
    
    do {
        MT = uphill = reject = 0;
        do {
            MT++;

            int type = rand() % 2 + 1;
            std::map<std::string, Info> new_state_encoding = perturb(cur_state_encoding, state_names, type);

            int cost = calculateCostFunc(new_state_encoding);
            int deltaCost = cost - curCost;

            double rd = (double) rand() / RAND_MAX;
            if ((deltaCost < 0) || rd < exp(-deltaCost / T)) {
                if (deltaCost > 0) uphill++;
                cur_state_encoding = new_state_encoding;
                curCost = cost;

                if (curCost < bestCost) {
                    // std::cout << "best cost: " << curCost << std::endl;
                    best_state_encoding = cur_state_encoding;
                    bestCost = curCost;
                }
            } else {
                reject++;
            }
        } while (uphill <= N && MT <= 2 * N);
        T *= r;

    } while ((reject / MT) < 0.95 && T >= e);
    // } while (false);

    // fill the best encoding to the state
    for (auto s : states) {
        State* state = s.second;
        state->encoded = best_state_encoding[state->name].encoded;
    }
}

std::map<std::string, Info> perturb(std::map<std::string, Info> &state_info, std::vector<std::string> &state_names, int type) {
    std::map<std::string, Info> new_state_info = state_info;
    int state1, state2;
    std::string neighbor1, neighbor2;

    switch(type) {
        // swap group
        case 1:
            state1 = rand() % state_names.size();
            do {
                state2 = rand() % state_names.size();
            } while (state_info[state_names[state1]].neighbor == state_names[state2]);

            neighbor1 = state_info[state_names[state1]].neighbor;
            neighbor2 = state_info[state_names[state2]].neighbor;

            // std::cout << "state1: " << state_names[state1] << " " << state_info[state_names[state1]].neighbor << " " << state_info[state_names[state1]].encoded << std::endl;
            // std::cout << "state2: " << state_names[state2] << " " << state_info[state_names[state2]].neighbor << " " << state_info[state_names[state2]].encoded << std::endl;
            // std::cout << "neighbor1: " << neighbor1 << " " << state_info[neighbor1].neighbor << " " << state_info[neighbor1].encoded << std::endl;
            // std::cout << "neighbor2: " << neighbor2 << " " << state_info[neighbor2].neighbor << " " << state_info[neighbor2].encoded << std::endl;

            std::swap(new_state_info[state_names[state1]].encoded, new_state_info[state_names[state2]].encoded);
            if (neighbor1 == state_names[state1]) {
                std::string new_encode = new_state_info[state_names[state2]].encoded;
                if (new_encode.back() == '0') {
                    // std::cout << "single state encoded bit is not 0" << std::endl;
                    new_encode.pop_back();
                    new_encode.push_back('1');
                } else {
                    new_encode.pop_back();
                    new_encode.push_back('0');
                }
                new_state_info[neighbor2].encoded = new_encode;
            } else if (neighbor2 == state_names[state2]) {
                std::string new_encode = new_state_info[state_names[state1]].encoded;
                if (new_encode.back() == '0') {
                    // std::cout << "single state encoded bit is not 0" << std::endl;
                    new_encode.pop_back();
                    new_encode.push_back('1');
                } else {
                    new_encode.pop_back();
                    new_encode.push_back('0');
                }
                new_state_info[neighbor1].encoded = new_encode;
            } else {
                std::swap(new_state_info[neighbor1].encoded, new_state_info[neighbor2].encoded);
            }

            // std::cout << "new state1: " << state_names[state1] << " " << new_state_info[state_names[state1]].neighbor << " " << new_state_info[state_names[state1]].encoded << std::endl;
            // std::cout << "new state2: " << state_names[state2] << " " << new_state_info[state_names[state2]].neighbor << " " << new_state_info[state_names[state2]].encoded << std::endl;
            // std::cout << "new neighbor1: " << neighbor1 << " " << new_state_info[neighbor1].neighbor << " " << new_state_info[neighbor1].encoded << std::endl;
            // std::cout << "new neighbor2: " << neighbor2 << " " << new_state_info[neighbor2].neighbor << " " << new_state_info[neighbor2].encoded << std::endl;
        break;

        // swap state
        case 2:
            state1 = rand() % state_names.size();
            do {
                state2 = rand() % state_names.size();
            } while (state1 == state2);

            std::swap(new_state_info[state_names[state1]].encoded, new_state_info[state_names[state2]].encoded);
            if (new_state_info[state_names[state1]].neighbor != state_names[state2]) {
                if (new_state_info[state_names[state1]].neighbor == state_names[state1]) {
                    std::string n2 = new_state_info[state_names[state2]].neighbor;
                    new_state_info[state_names[state1]].neighbor = n2;
                    new_state_info[state_names[state2]].neighbor = state_names[state2];
                    new_state_info[n2].neighbor = state_names[state1];
                } else if (new_state_info[state_names[state2]].neighbor == state_names[state2]) {
                    std::string n1 = new_state_info[state_names[state1]].neighbor;
                    new_state_info[state_names[state1]].neighbor = state_names[state1];
                    new_state_info[state_names[state2]].neighbor = n1;
                    new_state_info[n1].neighbor = state_names[state2];
                } else {
                    std::string n1 = new_state_info[state_names[state1]].neighbor;
                    std::string n2 = new_state_info[state_names[state2]].neighbor;
                    new_state_info[state_names[state1]].neighbor = n2;
                    new_state_info[state_names[state2]].neighbor = n1;
                    new_state_info[n1].neighbor = state_names[state2];
                    new_state_info[n2].neighbor = state_names[state1];
                }
            }
            
        break;  
        default:
            std::cout << "invalid type" << std::endl;
        break;
    }

    // std::cout << "Type: " << type << std::endl;
    // std::cout << "state: " << state_names[state1] << " " << state_names[state2] << std::endl;
    // for (auto it = new_state_info.begin(); it != new_state_info.end(); it++) {
    //     std::cout << it->first << " " << it->second.neighbor << " " << it->second.encoded << std::endl;
    // }
    // std::cout << std::endl;

    return new_state_info;
}

double calculateCostFunc(std::map<std::string, Info>& state_info) {
    double cost = 0;
    for (auto it = state_info.begin(); it != state_info.end(); it++) {
        for (auto it2 = state_info.begin(); it2 != state_info.end(); it2++) {
            if (it->first == it2->first) continue;
            std::string state_name1 = it->first;
            std::string state_name2 = it2->first;
            std::string transition_name = state_name1 + "_" + state_name2;
            if (cost_weights.find(transition_name) != cost_weights.end()) {
                // int w = weights[transition_name];
                double w = calculateWeight(state_name1, state_name2);
                int d = calculateDistance(it->second.encoded, it2->second.encoded);
                cost += w * (double)d;
            }
        }
    }

    // std::cout << "calculate cost finished" << std::endl;

    return cost;
}

int calculateDistance(std::string s1, std::string s2) {
    int n = s1.length();
    int m = s2.length();
    if (n != m) {
        std::cout << "Error: two encoded string have different length" << std::endl;
        return -1;
    }

    int dis = 0;
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i]) {
            dis++;
        }
    }

    // std::cout << "calculate distance finished" << std::endl;

    return dis;
}

double calculateWeight(std::string s1, std::string s2) {
    std::string cw_name = s1 + "_" + s2;
    return cost_weights[cw_name];
}

// */