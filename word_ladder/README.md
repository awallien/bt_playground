# Word Chain

This is technically a backtracking algorithm. 

Given a start and goal word of same length, it builds a chain of words
from the start word to the goal word by changing one letter
in each linkage.

For example, if the start word is "bike" and the goal word is "lamp",
the algorithm produces this chain: <br/> bike => like => lime => limp => lamp.

Sometimes, no chain would exist between two words. For example, if the
start word is "start" and the goal word is "apple", the algorithm would
not be able to find a chain. 

# Installation and Run

Python3.x is required. 

To run the program: `python3 word_change.py dict_file` where `dict_file` is a file of dictionary
words, one per line. See examples of the dictionary files under `dict` directory