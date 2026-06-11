# Question 1: Max-Heap Job Scheduler



---

## Problem Description

In a cloud computing environment, incoming jobs are handled by a **resource manager**. Each job is assigned a **priority score**, and the system must always execute the job with the **highest priority first**.

To support fast insertion and removal of jobs, the scheduler internally uses a **Max-Heap** data structure.

You are given the following list of job priorities received by the scheduler:

```
P = {42, 17, 93, 28, 65, 81, 54, 60, 99, 73, 88}
```

---

## Tasks

### Task 1: Build the binary tree and assign job IDs

- Build a binary tree from the given array `P` (level-order / array representation).
- Assign each job a **unique identifier** using a single uppercase letter (A, B, C, …).
- Each heap node must store:
  - **Job ID**
  - **Job priority**

### Task 2: Convert to a Max-Heap

- Convert the binary tree into a valid **Max-Heap**.
- Use **bottom-up heapify** (`heapify-down` from the last non-leaf node to the root).
- Show the heap structure after conversion.

### Task 3: Insert urgent job (priority 100)

- A new urgent job with **priority 100** arrives.
- Insert this job into the heap.
- Re-adjust the structure using **heapify-up** to maintain the Max-Heap property.
- Show the heap structure after insertion.

### Task 4: Remove the urgent job (priority 100)

- The urgent job is later **aborted**.
- Remove the job with **priority 100** from the heap.
- Replace it with the last element and re-adjust using **heapify-down**.
- Show the heap structure after removal.

---

## Constraints

| Requirement | Implementation |
|-------------|----------------|
| Language | C |
| Heap type | Array-based binary max-heap |
| Insertion | Proper `heapify-up` (bubble up) |
| Deletion | Proper `heapify-down` (sink down) |
| Output | Heap structure shown after each major operation |

---

## Project Files

| File | Description |
|------|-------------|
| `max_heap_scheduler.c` | Main C source code |
| `output.txt` | Program output (auto-generated on run) |
| `Makefile` | Optional build helper |
| `README.MD` | This documentation |

---

## How to Compile and Run

### Windows (PowerShell)

```powershell
cd "question 1"
gcc -Wall -Wextra -std=c11 -o max_heap_scheduler.exe max_heap_scheduler.c
.\max_heap_scheduler.exe
```

### Using Make (optional)

```bash
make
make run
```

After running, results are printed to the terminal and saved to **`output.txt`**.

---

## Job Identifier Mapping

Each array index is mapped to a unique job ID when the heap is first built:

| Index | Job ID | Priority |
|-------|--------|----------|
| 0 | A | 42 |
| 1 | B | 17 |
| 2 | C | 93 |
| 3 | D | 28 |
| 4 | E | 65 |
| 5 | F | 81 |
| 6 | G | 54 |
| 7 | H | 60 |
| 8 | I | 99 |
| 9 | J | 73 |
| 10 | K | 88 |
| 11 | L | 100 *(inserted urgent job)* |

---

## Initial Binary Tree (before heapify)

Level-order array representation:

```
[A(42), B(17), C(93), D(28), E(65), F(81), G(54), H(60), I(99), J(73), K(88)]
```

Tree layout:

```
                    A(42)
                   /     \
              B(17)       C(93)
             /    \      /    \
        D(28)  E(65) F(81)  G(54)
        /  \   /  \   /  \
    H(60) I(99) J(73) K(88)
```

---

## Results Summary

### After Task 2 — Max-Heap built

**Array:**
```
[I(99), K(88), C(93), H(60), J(73), F(81), G(54), B(17), D(28), A(42), E(65)]
```

**Heapify-down swaps (build phase):**
- Index 4: E(65) ↔ K(88)
- Index 3: D(28) ↔ I(99)
- Index 2: C(93) — no swap needed
- Index 1: B(17) ↔ I(99), then B(17) ↔ H(60)
- Index 0: A(42) ↔ I(99), then A(42) ↔ K(88), then A(42) ↔ J(73)

**Max-heap check:** VALID

---

### After Task 3 — Insert L(100)

**Steps:**
1. Append L(100) at index 11.
2. Heapify-up:
   - L(100) ↔ F(81) at index 5
   - L(100) ↔ C(93) at index 2
   - L(100) ↔ I(99) at index 0

**Final array:**
```
[L(100), K(88), I(99), H(60), J(73), C(93), G(54), B(17), D(28), A(42), E(65), F(81)]
```

**Max-heap check:** VALID

---

### After Task 4 — Remove priority 100

**Steps:**
1. Replace root L(100) with last element F(81).
2. Heapify-down from root:
   - F(81) ↔ I(99) at index 2
   - F(81) ↔ C(93) at index 5

**Final array:**
```
[I(99), K(88), C(93), H(60), J(73), F(81), G(54), B(17), D(28), A(42), E(65)]
```

**Max-heap check:** VALID

---

## Implementation Details

### Data structures

```c
typedef struct {
    char id;        /* Job ID: A, B, C, ... */
    int priority;   /* Job priority score   */
} Job;

typedef struct {
    Job data[MAX_JOBS];  /* Array-based heap */
    int size;            /* Number of jobs   */
} MaxHeap;
```

### Array-based heap indexing

| Operation | Formula |
|-----------|---------|
| Parent of index `i` | `(i - 1) / 2` |
| Left child of `i` | `2 * i + 1` |
| Right child of `i` | `2 * i + 2` |
| Last non-leaf index | `(size / 2) - 1` |

### Core operations

| Function | Purpose |
|----------|---------|
| `build_max_heap()` | Bottom-up heapify to create max-heap |
| `heapify_down()` | Restore heap after deletion or during build |
| `heapify_up()` | Restore heap after insertion |
| `insert_job()` | Append job and bubble up |
| `remove_job_by_priority()` | Replace node with last element and re-heapify |
| `verify_max_heap()` | Confirms max-heap property holds |

---

## Grading Rubric Alignment

| Criterion | Status |
|-----------|--------|
| Binary tree correctly constructed from array P | Done |
| Converted to valid Max-Heap | Done |
| Unique job IDs assigned with priorities | Done |
| Priority 100 inserted with correct heapify-up | Done |
| Priority 100 removed with correct heapify-down | Done |
| Heap ordering correct after every operation | Verified (VALID) |
| Implemented in C with array-based logic | Done |
| Heap structure shown after each major step | Done |

---

## Submission

Submit the following as required by your instructor:

1. **`max_heap_scheduler.c`** — source code
2. **`output.txt`** — full program output showing all heapify steps and heap states

Re-run the program before submitting to ensure `output.txt` is up to date:

```powershell
.\max_heap_scheduler.exe
```
