// Copyright Vu Nguyen 2024
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

int numStates, alphabetSize;
set<int> acceptingStates;

// A structure to represent a state and its transitions
struct State {
  set<int> epsilonTransitions;     // Stores ε-transitions
  map<char, set<int> > transitions;  // Stores non-ε transitions
};

// Utility function to extract a number from a line with descriptive text
int extractNumberFromLine(const std::string &line) {
  std::stringstream ss(line);
  std::string temp;
  int number;
  while (ss >> temp) {
    std::stringstream tempStream(temp);
    if (tempStream >> number) {
      return number;
    }
  }
  return -1;
}

vector<State> readENFA(const string &filename) {
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

  for (int i = 0; getline(file, line) && i < numStates; ++i) {
    stringstream ss(line);
    string epsilonTransitions, transition;
    ss >> epsilonTransitions;

    stringstream transStream(
        epsilonTransitions.substr(1, epsilonTransitions.size() - 2));
    string target;
    while (getline(transStream, target, ',')) {
      if (!target.empty())
        enfa[i].epsilonTransitions.insert(stoi(target));
    }

    for (char symbol = 'a'; symbol < 'a' + alphabetSize; ++symbol) {
      ss >> transition;
      stringstream transStream2(transition.substr(1, transition.size() - 2));
      while (getline(transStream2, target, ',')) {
        if (!target.empty())
          enfa[i].transitions[symbol].insert(stoi(target));
      }
    }
  }
  file.close();
  return enfa;
}

void dfsEClosure(int currentState, const vector<State> &enfa,
                 set<int> &closure) {
  if (closure.find(currentState) == closure.end()) {
    closure.insert(currentState);
    for (set<int>::iterator it = enfa[currentState].epsilonTransitions.begin();
         it != enfa[currentState].epsilonTransitions.end(); ++it) {
      dfsEClosure(*it, enfa, closure);
    }
  }
}

set<int> computeEClosure(int state, const vector<State> &enfa) {
  set<int> closure;
  dfsEClosure(state, enfa, closure);
  return closure;
}

vector<State> convertToNFA(const vector<State> &enfa) {
  vector<State> nfa(enfa.size());
  vector<set<int> > eClosures(numStates);
  // Compute ε-closures for all states.
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
      for (set<int>::iterator it = eClosures[state].begin();
           it != eClosures[state].end(); ++it) {
        map<char, set<int> >::const_iterator directTrans =
            enfa[*it].transitions.find(symbol);
        if (directTrans != enfa[*it].transitions.end()) {
          for (set<int>::const_iterator targetIt = directTrans->second.begin();
               targetIt != directTrans->second.end(); ++targetIt) {
            newTransitions.insert(*targetIt);
          }
        }
      }
      nfa[state].transitions[symbol] = newTransitions;
    }
  }

  // Update accepting states based on ε-closures.
  set<int> updatedAcceptingStates;
  for (int i = 0; i < numStates; ++i) {
    for (set<int>::const_iterator accState = acceptingStates.begin();
         accState != acceptingStates.end(); ++accState) {
      if (eClosures[i].find(*accState) != eClosures[i].end()) {
        updatedAcceptingStates.insert(i);
        break;
      }
    }
  }
  acceptingStates = updatedAcceptingStates;
  return nfa;
}

void outputNFA(const vector<State> &nfa) {
  cout << "Number of states: " << nfa.size() << endl;
  cout << "Alphabet size: " << alphabetSize << endl;
  cout << "Accepting states:";
  for (set<int>::iterator state = acceptingStates.begin();
       state != acceptingStates.end(); ++state)
    cout << " " << *state;
  cout << endl;

  for (int i = 0; i < numStates; ++i) {
    cout << "{}";
    for (char symbol = 'a'; symbol < 'a' + alphabetSize; ++symbol) {
      cout << " {";
      const set<int> &transSet = nfa[i].transitions.at(symbol);
      bool first = true;
      for (set<int>::const_iterator targetState = transSet.begin();
           targetState != transSet.end(); ++targetState) {
        if (!first)
          cout << ",";
        cout << *targetState;
        first = false;
      }
      cout << "}";
    }
    cout << endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <ε-NFA description file>" << endl;
    return 1;
  }

  string filename = argv[1];
  vector<State> enfa = readENFA(filename);
  if (enfa.empty()) {
    cerr << "Failed to read ε-NFA from file." << endl;
    return 2;
  }

  vector<State> nfa = convertToNFA(enfa);
  outputNFA(nfa);

  return 0;
}
