// Copyright Vu Nguyen 2024
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class NFA {
 public:
  int numStates;
  set<int> acceptingStates;
  vector<map<char, set<int> > > transitions;

  NFA(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
      cerr << "Error opening file: " << filename << endl;
      exit(1);
    }

    string line;
    getline(file, line);  // Read number of states
    numStates = atoi(line.substr(line.find(':') + 1).c_str());

    getline(file, line);  // Read alphabet size
    int alphabetSize = atoi(line.substr(line.find(':') + 1).c_str());

    getline(file, line);  // Read accepting states
    parseAcceptingStates(line.substr(line.find(':') + 1));

    transitions.resize(numStates);
    int stateIndex = 0;
    while (getline(file, line) && stateIndex < numStates) {
      parseTransitions(stateIndex, line, alphabetSize);
      stateIndex++;
    }
  }

  void parseAcceptingStates(const string &line) {
    istringstream stream(line);
    int state;
    while (stream >> state) {
      acceptingStates.insert(state);
    }
  }

  void parseTransitions(int state, const string &line, int alphabetSize) {
    istringstream stream(line);
    string part;
    char symbol = 'a';  // Starting symbol in the alphabet

    // Skip the first empty set representing ε-transitions
    getline(stream, part, '}');  // Move past the first empty set

    int count = 0;
    while (getline(stream, part, '{') && getline(stream, part, '}')) {
      istringstream setStream(part);
      int nextState;
      while (setStream >> nextState) {
        transitions[state][symbol].insert(nextState);
        if (setStream.peek() == ',')
          setStream.ignore();
      }
      if (++count < alphabetSize) {
        symbol++;  // Increment to next alphabet character
      }
    }
  }

  bool simulate(const string &input) {
    set<int> currentStates;
    currentStates.insert(0);  // Start from state 0

    for (size_t i = 0; i < input.size(); ++i) {
      char c = input[i];
      set<int> nextStates;
      for (set<int>::iterator it = currentStates.begin();
           it != currentStates.end(); ++it) {
        int state = *it;
        if (transitions[state].find(c) != transitions[state].end()) {
          nextStates.insert(transitions[state][c].begin(),
                            transitions[state][c].end());
        }
      }
      if (nextStates.empty()) {
        return false;
      }
      currentStates = nextStates;
    }

    for (set<int>::iterator it = currentStates.begin();
         it != currentStates.end(); ++it) {
      if (acceptingStates.find(*it) != acceptingStates.end()) {
        return true;
      }
    }
    return false;
  }
};

int main(int argc, char *argv[]) {
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
