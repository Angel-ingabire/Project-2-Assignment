# Question 5 — Bellman–Ford shortest paths from Branch A

This folder contains a C program implementing the Bellman–Ford algorithm to compute shortest transaction cost paths from Branch A to all other branches based on the provided inter-branch graph.

Files:

- `bellman_ford.c` — C source implementing Bellman–Ford, negative-cycle detection, and path output
- `Makefile` — build helper (produces `question5_bf.exe`)

Build & run (PowerShell):

```
make
.\question5_bf.exe
```

The program prints the graph edges, detects negative-weight cycles (if any), and then prints the shortest path and minimum cumulative cost from Branch A to every branch.
