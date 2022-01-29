#Magic maze solver
The goal is to find shortest possible path in all possible combinations of the maze.
Maze can be affected by magic levers, which can be pulled before entering the maze. Lever applies binary xor to the whole maze. Walk to push the lever from and to starting position also counts to the total path length.


Input format:
n k
k rows of lever combinations.
    l(i) b
maze
x y

where n is size of square maze and k is number of magic levers.
1 <= n 1000
0 <= k 10

l is the vertical position of the lever from the starting position
-1 <= l <= n 

b is sequence of n numbers from {0, 1}. States the lever change effect

maze is defined by n^2 numbers from {0, 1} where 0 is free tile and 1 is obstacle

x y is the maze exit
1 <= x, y <= n



Eample Input 1:

5 1
5 00001
01000 00000 00000 00000 10001
5 5


Output:

18
1
[1;1],[1;2],[1;3],[1;4],[2;4],[2;5],[3;5],[4;5],[5;5]



Eample Input 2:

In:
5 0
01000 00000 00000 00000 00000
3 1



Output:

4
[1;1],[1;2],[2;2],[3;2],[3;1]


Coded for BI-AG1 class 2021
