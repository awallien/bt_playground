"""
    file:	word_change.py
    author: Alexander Wall
    description:
        program that finds the shortest change length of words that differ by one letter,
        given the start and goal words
    date: 1/30/2020
    revision: 1.0
"""
from os.path import isfile
from sys import argv


class Dictionary:
    """valid characters to put into a word, assuming no contractions and punctuations"""
    valid_chars = [chr(i) for i in range(ord('A'), ord('Z') + 1)] + \
                  [chr(i) for i in range(ord('a'), ord('z') + 1)] + \
                  ['\'']

    @classmethod
    def get_dict(cls, word_len) -> set:
        """make a dictionary from given file
        :param word_len: given length for a word
        :return: a set of dictionary
        """
        with open(argv[1]) as fp:
            d = set()
            for line in fp:
                line = line.strip()
                if len(line) == word_len:
                    d.add(line)
            return d


class Node:
    def __init__(self, word, depth, goal, predecessor):
        """Constructor"""
        self.word = word
        self.depth = depth
        self.goal = goal
        self.predecessor = predecessor
        self.h = self.__h()

    def __h(self):
        """helper function to calculate the number of similar characters between
        the temp and the goal word
        :return: a number
        """
        return sum([1 for i in range(len(self.word)) if self.word[i] == self.goal[i]])

    def get_successors(self, dictionary):
        """get the successors for a word where each successor is changed by one letter
        :param dictionary: collection of words with same length as self.word
        :return: list of successors
        """
        res = list()
        for i in range(len(self.word)):
            left = self.word[:i]
            right = self.word[i + 1:]
            for a in Dictionary.valid_chars:
                temp = left + a + right
                if temp in dictionary and temp != self.word:
                    res.append(Node(temp, self.depth + 1, self.goal, self))
        return sorted(res, key=lambda x: [x.h, x.word], reverse=True)

    def __hash__(self):
        """for the purpose of adding this Node to a set"""
        return hash(self.word)

    def __repr__(self):
        """for the purpose of debugging"""
        return self.__str__()

    def __eq__(self, other):
        """for the purpose of comparing two Nodes"""
        if type(other) == Node:
            return other.word == self.word
        return False


class Search:
    def __init__(self, start, goal):
        """start and goal words"""
        self.start: str = start
        self.goal: str = goal

        """the overall chain from start word to goal word"""
        self.chain: [Node, None] = None

        """the limit on how far the search would go in depth"""
        self.limit = len(start)**2

        """collection of visited words at a depth where dict[word] = depth"""
        self.visited = dict()

        """collection of words with the same length as the starting or goal word"""
        self.dictionary = Dictionary.get_dict(len(self.start))

        self.__search()

    def __search(self):
        """initialize the search"""
        start_node = Node(self.start, 0, self.goal, None)
        self.visited[start_node] = 0
        self.__DLS(start_node)

    def __DLS(self, word: Node):
        """DFS but with a limit search when a better goal is found aka DLS"""
        if word.word == self.goal and (not self.chain or word.depth < self.chain.depth):
            self.chain = word
            self.limit = word.depth
        elif word.depth <= self.limit:
            for successor in word.get_successors(self.dictionary):
                if successor not in self.visited or self.visited[successor] > successor.depth:
                    self.visited[successor] = successor.depth
                    self.__DLS(successor)

    def print_result(self) -> None:
        """print chain of words to output"""
        c: Node = self.chain
        if c is None:
            print("No chain found!")
        else:
            res = ""
            while c.predecessor is not None:
                res = " => " + c.word + res
                c = c.predecessor
            print(c.word + res)


def main() -> None:
    """gets start and goal words from the user and print result, also includes default search words"""
    for default in [["cold", "warm"], ["small", "short"]]:
        resp = input("Would you like to see the result for search(\"%s\", \"%s\")? (Y/N): " % (default[0], default[1]))
        if resp == 'Y':
            Search(default[0], default[1]).print_result()

    resp = input("Would you like to try one? (Y/N): ")
    if resp == 'Y':
        word1 = input("    Enter starting word: ")
        word2 = input("        Enter goal word: ")
        assert len(word1) == len(word2), "%s and %s have different lengths" % (word1, word2)
        Search(word1, word2).print_result()
    print("Goodbye!")


if __name__ == '__main__':
    if len(argv) != 2:
        print("Usage: python3 word_change.py [dict-file]")
    elif not isfile(argv[1]):
        print("Dictionary does not exist: ", argv[1])
    else:
        main()
