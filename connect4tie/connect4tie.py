"""

file: 
    connect4tie.py

description: 
    use backtracking to count the number of connect four tie boards
    (there's probably a mathematical formula for this...)

author: 
    awallien

date: 
    4/25/19

output: 
    713305820

"""

"""
representation of the board
"""
board = [' ' for i in range(42)]

"""
for counting the number of connect four tie board
"""
count = 0

"""
check if all the values in list are the same value
"""
_same = lambda lst: all(lst[0] == x for x in lst)

"""
check if at least one of the three lists have the same value
"""
checkall = lambda i, j, k: _same(i) or _same(j) or _same(k)

"""
When done populating the whole board, check if whole board is valid
"""
is_goal = lambda i: i == 42 and board.count("X") == board.count("O")


def bt(i=0):
    """
    Backtracking function to count all the connect four tie boards
    :param i: the current index on the board
    :return: True if goal is reached in backtracking; false otherwise
             does not necessarily return anything if you think about it
    """
    global count
    if is_goal(i):
        return True
    elif i < 42:
        for c in ["X", "O"]:
            board[i] = c
            if is_valid(i) and bt(i + 1):
                count += 1
                #print(to_string())
        board[i] = ' '

        if i == 0:
            return False


"""
Following functions check for non-winning configurations on the board from a
given 'i' index

UR - upper right block of the board
BL - bottom left area
BR - bottom right area
MC - middle column
"""
def check_UR(i):
    check_lst1 = board[i - 3:i + 1]
    return _same(check_lst1)

def check_BL(i):
    check_lst2 = [board[i], board[i - 7], board[i - 14], board[i - 21]]
    check_lst3 = [board[i], board[i - 6], board[i - 12], board[i - 18]]

    return _same(check_lst2) or _same(check_lst3)

def check_BR(i):
    check_lst1 = board[i - 3:i + 1]
    check_lst2 = [board[i], board[i - 7], board[i - 14], board[i - 21]]
    check_lst3 = [board[i], board[i - 8], board[i - 16], board[i - 24]]

    return checkall(check_lst1, check_lst2, check_lst3)

def check_MC(i):
    if i in [3,10,17]:
        return _same(board[i-3:i+1])
    if i in [24,31,38]:
        return check_BR(i) or _same([board[i], board[i - 6], board[i - 12], board[i - 18]])


def is_valid(i):
    """
    checks whether a piece put down on the board is a valid tie board
    :param i: the index to put down a piece
    :return: True if valid; otherwise, false
    """
    if i in [0, 1, 2, 7, 8, 9, 14, 15, 16]:
        return True

    if i in [4, 5, 6, 11, 12, 13, 18, 19, 20] and not check_UR(i):
        return True

    if i in [21, 22, 23, 28, 29, 30, 35, 36, 37] and not check_BL(i):
        return True

    if i in [25, 26, 27, 32, 33, 34, 39, 40, 41] and not check_BR(i):
        return True

    if i in [3, 10, 17, 24, 31, 38] and not check_MC(i):
        return True

    return False


def to_string():
    """
     string representation of the connect 4 board
    """
    res = ""
    for i in range(6):
        for j in range(7):
            res += board[i*7+j]
        res += "\n"
    return res[::-1]


if __name__ == '__main__':
    bt()
    print(count)

