class NFA:
    def __init__(self):
        # Initialize transitions for each state for both 'a' and 'b' with default empty sets
        self.transitions = {
            0: {'a': {2, 5}, 'b': set()},
            1: {'a': {2}, 'b': set()},
            2: {'a': set(), 'b': {3}},
            3: {'a': {2}, 'b': set()},
            4: {'a': {5}, 'b': set()},
            5: {'a': set(), 'b': {6}},
            6: {'a': {7}, 'b': set()},
            7: {'a': {5}, 'b': set()}
        }
        # Define accepting states
        self.accepting_states = {0, 1, 3, 4, 7}

    def simulate(self, input_string):
        current_states = {0}  # Start from state 0, and include handling for empty input
        if not input_string:  # If the input is empty, immediately check for state 0 acceptance
            return 'accept' if 0 in self.accepting_states else 'reject'
        
        for char in input_string:
            next_states = set()
            for state in current_states:
                if char in self.transitions[state]:
                    next_states.update(self.transitions[state][char])
            if not next_states:  # No transition possible from current states for this character
                return 'reject'
            current_states = next_states
        
        # Check if any of the resulting states are accepting
        return 'accept' if any(state in self.accepting_states for state in current_states) else 'reject'

# Define the NFA
nfa = NFA()

# Input strings to simulate
input_strings = [
    "aba", "", "a", "ab", "aba", "ababab", "abababa", "abaabaaba", "b", "bb", "ababab", "abaab"
]

# Simulate each string and print the result
for input_string in input_strings:
    result = nfa.simulate(input_string)
    print(f"Input: '{input_string}' -> Result: {result}")
