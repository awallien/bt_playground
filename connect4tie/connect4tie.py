"""

file: 
    connect4tie.py

description: 
    use backtracking to count the number of connect four tie boards

author: 
    awallien

date: 
    4/25/19

output: 
    713305820

"""

board = [' ' for i in range(42)]
count = 0

_same = lambda lst: all(lst[0] == x for x in lst)
checkall = lambda i, j, k: _same(i) or _same(j) or _same(k)
is_goal = lambda i: i == 42 and board.count("X") == board.count("O")


def bt(i=0):
    global count
    if is_goal(i):
        return True
    elif i < 42:
        for c in ["X", "O"]:
            board[i] = c
            if isValid(i) and bt(i + 1):
                count += 1
                #print(to_string())
        board[i] = ' '

        if i == 0:
            return False


def checkUR(i):
    check_lst1 = board[i - 3:i + 1]

    return _same(check_lst1)

def checkBL(i):
    check_lst2 = [board[i], board[i - 7], board[i - 14], board[i - 21]]
    check_lst3 = [board[i], board[i - 6], board[i - 12], board[i - 18]]

    return _same(check_lst2) or _same(check_lst3)

def checkBR(i):
    check_lst1 = board[i - 3:i + 1]
    check_lst2 = [board[i], board[i - 7], board[i - 14], board[i - 21]]
    check_lst3 = [board[i], board[i - 8], board[i - 16], board[i - 24]]

    return checkall(check_lst1, check_lst2, check_lst3)


def checkMC(i):
    if i in [3,10,17]:
        return _same(board[i-3:i+1])
    if i in [24,31,38]:
        return checkBR(i) or _same([board[i], board[i - 6], board[i - 12], board[i - 18]])


def isValid(i):
    if i in [0, 1, 2, 7, 8, 9, 14, 15, 16]:
        return True

    if i in [4, 5, 6, 11, 12, 13, 18, 19, 20] and not checkUR(i):
        return True

    if i in [21, 22, 23, 28, 29, 30, 35, 36, 37] and not checkBL(i):
        return True

    if i in [25, 26, 27, 32, 33, 34, 39, 40, 41] and not checkBR(i):
        return True

    if i in [3, 10, 17, 24, 31, 38] and not checkMC(i):
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

