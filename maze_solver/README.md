## Magic maze solver
<br>
The goal is to find shortest possible path in all possible combinations of the maze.
Maze can be affected by magic levers, which can be pulled before entering the maze. Lever applies binary xor to the whole maze. Walk to push the lever from and to starting position also counts to the total path length.

<br>

***
<b>Input format:</b><br>
n k
<br>
k rows of lever combinations - l(i) b
    <br>
maze
<br>
x y
<br><br>

where n is size of square maze and k is number of magic levers.
<br>
1 <= n 1000
<br>
0 <= k 10
<br><br>

l is the vertical position of the lever from the starting position
<br>
-1 <= l <= n

b is sequence of n numbers from {0, 1}. States the lever change effect
<br>

maze is defined by n^2 numbers from {0, 1} where 0 is free tile and 1 is obstacle
<br>

x y is the maze exit
<br>
1 <= x, y <= n
<br><br>

***

**Eample 1:**

Input:
```
5 1
5 00001
01000 00000 00000 00000 10001
5 5
```

Output:
```
18
1
[1;1],[1;2],[1;3],[1;4],[2;4],[2;5],[3;5],[4;5],[5;5]
```


**Eample 2:**

Input:
```
In:
5 0 
01000 00000 00000 00000 00000 
3 1 
```


Output:
```
4
[1;1],[1;2],[2;2],[3;2],[3;1]
```

***

Coded for BI-AG1 class 2021
