# Question 3 — Minimum Spanning Tree (Kruskal) in C

This folder contains a C implementation that models the given fiber-optic network as an adjacency matrix and applies Kruskal's algorithm to compute the Minimum Spanning Tree (MST).

Files:
- `mst.c` — C source implementing adjacency matrix + Kruskal (union-find)
- `Makefile` — build helper (produces `question3_mst.exe`)

Build and run (Windows PowerShell):
```
make
.\question3_mst.exe
```

The program prints the adjacency matrix, the selected MST edges, and the total installation cost (in millions).
