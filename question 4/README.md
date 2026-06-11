# Question 4 — BFS immediate contacts and highest-risk neighbor

This folder contains a C program that accepts a suspected compromised workstation (A-G), performs a BFS from that node to discover direct (one-hop) network peers, and reports which direct peer has the highest data transfer time (highest risk).

Files:

- `bfs.c` — C source that reads user input, runs BFS, lists immediate contacts and highest-risk contact
- `Makefile` — build helper (produces `question4_bfs.exe`)

Build & run (PowerShell):

```
make
echo D | .\question4_bfs.exe
```

Replace `D` with the suspected workstation letter.
