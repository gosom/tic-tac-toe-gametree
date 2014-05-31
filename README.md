tic-tac-toe-gametree
====================

Computes the entire tictactoe game tree.
Prints the number of nodes
Prints the branching factor

How to build
===

`g++ main.cpp -o compute-gametree`


Tested with 
g++ (GCC) 4.8.2 20131212 (Red Hat 4.8.2-7)

File compute-gametree is a binary for linux 32bit.
File results contains the output of the program

Output
===

```
- initiating tree traversal...
- tree traversal completed successfully...

- total number of nodes: 549946
- number of unique nodes: 5478
- number of X wins: 131184
- number of O wins: 77904
- number of draws: 46080

- number of nodes at tree depth 0: 1
- number of nodes at tree depth 1: 9
- number of nodes at tree depth 2: 72
- number of nodes at tree depth 3: 504
- number of nodes at tree depth 4: 3024
- number of nodes at tree depth 5: 15120
- number of nodes at tree depth 6: 54720
- number of nodes at tree depth 7: 148176
- number of nodes at tree depth 8: 200448
- number of nodes at tree depth 9: 127872

- average tree branching factor: 4.81307
```


Credits
====

Dimitris




