#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

struct State {
    set<int> epsilonTransitions;
    map<char, set<int>> transitions;
};

int numStates, alphabetSize;
set<int> acceptingStates;

int extractNumberFromLine(const string& line) {
    stringstream ss(line);
    string temp;
    int number;
    while (ss >> temp) {
        if (stringstream(temp) >> number) {
            return number;
        }
    }
    return -1;
}

vector<State> readENFA(const string& filename) {
    ifstream file(filename);
    vector<State> enfa;
    string line;
    if (!file.is_open()) {
        cerr << "Could not open the file.\n";
        return enfa;
    }

    // Read number of states
    getline(file, line);
    numStates = extractNumberFromLine("Number of states: " + line);

    // Read alphabet size
    getline(file, line);
    alphabetSize = extractNumberFromLine("Alphabet size: " + line);

    // Read accepting states
    getline(file, line);
    stringstream ss(line.substr(17));  // Skip "Accepting states: "
    int state;
    while (ss >> state) {
        acceptingStates.insert(state);
    }

    // Initialize states
    enfa.resize(numStates);

    auto parseTransitions = [](const string& transStr) -> set<int> {
        set<int> transitions;
        if (transStr == "{}") return transitions;
        stringstream transStream(transStr.substr(1, transStr.size() - 2));
        string target;
        while (getline(transStream, target, ',')) {
            if (!target.empty()) transitions.insert(stoi(target));
        }
        return transitions;
    };

    for (int i = 0; getline(file, line) && i < numStates; ++i) {
        stringstream ss(line);
        string epsilonTransitions, transition;
        ss >> epsilonTransitions;
        enfa[i].epsilonTransitions = parseTransitions(epsilonTransitions);
        for (char symbol = 'a'; symbol < 'a' + alphabetSize; ++symbol) {
            ss >> transition;
            enfa[i].transitions[symbol] = parseTransitions(transition);
        }
    }
    file.close();
    return enfa;
}

void dfsEClosure(int currentState, const vector<State>& enfa, set<int>& closure) {
    if (closure.insert(currentState).second) {
        closure.insert(currentState);
        for (int nextState : enfa[currentState].epsilonTransitions) {
            dfsEClosure(nextState, enfa, closure);
        }
    }
}

set<int> computeEClosure(int state, const vector<State>& enfa) {
    set<int> closure;
    dfsEClosure(state, enfa, closure);
    return closure;
}

vector<State> convertToNFA(const vector<State>& enfa) {
    vector<State> nfa(enfa.size());
    vector<set<int>> eClosures(numStates);
    for (int i = 0; i < numStates; ++i) {
        eClosures[i] = computeEClosure(i, enfa);
    }

    // Initialize NFA states with empty transitions.
    for (size_t i = 0; i < nfa.size(); ++i) {
        for (char symbol = 'a'; symbol < 'a' + alphabetSize; ++symbol) {
            nfa[i].transitions[symbol] = set<int>();
        }
    }

    for (size_t state = 0; state < enfa.size(); ++state) {
        for (char symbol = 'a'; symbol < 'a' + alphabetSize; ++symbol) {
            set<int> newTransitions;
            for (const int closureState : eClosures[state]) {
                const auto& directTrans = enfa[closureState].transitions.find(symbol);
                if (directTrans != enfa[closureState].transitions.end()) {
                    for (int directlyReachableState : directTrans->second) {
                        newTransitions.insert(directlyReachableState);
                    }
                }
            }
            nfa[state].transitions[symbol] = std::move(newTransitions);
        }
    }

    set<int> updatedAcceptingStates;
    for (int i = 0; i < numStates; ++i) {
        for (int accState : acceptingStates) {
            if (eClosures[i].find(accState) != eClosures[i].end()) {
                updatedAcceptingStates.insert(i);
                break;
            }
        }
    }
    acceptingStates = std::move(updatedAcceptingStates);
    return nfa;
}

void outputNFA(const vector<State>& nfa) {
    cout << "Number of states: " << numStates << endl;
    cout << "Alphabet size: " << alphabetSize << endl;
    cout << "Accepting states:";
    for (int state : acceptingStates) cout << " " << state;
    cout << endl;

    for (int i = 0; i < numStates; ++i) {
        cout << "{}";
        for (char symbol = 'a'; symbol < 'a' + alphabetSize; ++symbol) {
            cout << " {";
            const auto& transSet = nfa[i].transitions.at(symbol);
            bool first = true;
            for (int targetState : transSet) {
                if (!first) cout << ",";
                cout << targetState;
                first = false;
            }
            cout << "}";
        }
        cout << endl;
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <ε-NFA description file>" << endl;
        return 1;
    }

    vector<State> enfa = readENFA(argv[1]);
    if (enfa.empty()) {
        return 2;
    }

    vector<State> nfa = convertToNFA(enfa);
    outputNFA(nfa);

    return 0;
}
