#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <sstream>

using namespace std;

// Represents an NFA
class NFA {
public:
    int numStates;
    set<int> acceptingStates;
    map<int, map<char, set<int>>> transitions;

    // Constructor to initialize NFA from a file
    NFA(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            exit(1);
        }

        string line;
        if (getline(file, line)) {
            numStates = stoi(line.substr(line.find(':') + 1));
        }
        if (getline(file, line)) {
            // Read alphabet size, not used directly in this implementation
        }
        if (getline(file, line)) {
            stringstream ss(line.substr(line.find(':') + 1));
            int state;
            while (ss >> state) {
                acceptingStates.insert(state);
            }
        }
        int state = 0;

        // Read transitions
        while (getline(file, line)) {
            int currentState = stoi(line.substr(0, line.find(' ')));
            char inputChar = line[line.find(' ') + 1];
            string transitionsString = line.substr(line.find('{') + 1, line.find('}') - line.find('{') - 1);
            stringstream transStream(transitionsString);
            string nextState;
            while (getline(transStream, nextState, ',')) {
                transitions[currentState][inputChar].insert(stoi(nextState));
            }
        }
    }

    // Simulate the NFA with an input string
    bool simulate(const string& input) {
        set<int> currentStates = {0};  // Assuming state 0 is the start state
        for (char c : input) {
            set<int> nextStates;
            for (int state : currentStates) {
                if (transitions[state].find(c) != transitions[state].end()) {
                    nextStates.insert(transitions[state][c].begin(), transitions[state][c].end());
                }
            }
            if (nextStates.empty()) return false;
            currentStates = nextStates;
        }

        // Check for any accepting state in currentStates
        for (int state : currentStates) {
            if (acceptingStates.find(state) != acceptingStates.end()) {
                return true;
            }
        }
        return false;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <NFA file>" << endl;
        return 1;
    }

    NFA nfa(argv[1]);

    string input;
    while (getline(cin, input)) {
        cout << (nfa.simulate(input) ? "accept" : "reject") << endl;
    }

    return 0;
}
