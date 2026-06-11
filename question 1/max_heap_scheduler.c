/*
 * Question 1: Max-Heap Job Scheduler
 *
 * Builds a max-heap from job priorities, inserts priority 100,
 * then removes it while showing heapify steps after each operation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_JOBS 64
#define OUTPUT_FILE "output.txt"

static FILE *g_output_file = NULL;

static void tee_printf(const char *format, ...) {
    va_list args;

    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    if (g_output_file != NULL) {
        va_start(args, format);
        vfprintf(g_output_file, format, args);
        va_end(args);
    }
}

#define printf tee_printf

typedef struct {
    char id;
    int priority;
} Job;

typedef struct {
    Job data[MAX_JOBS];
    int size;
} MaxHeap;

static void print_job(Job job) {
    printf("%c(%d)", job.id, job.priority);
}

static void print_heap_array(MaxHeap *heap) {
    int i;

    printf("  Array (level-order): [");
    for (i = 0; i < heap->size; i++) {
        print_job(heap->data[i]);
        if (i < heap->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

static void print_binary_tree(MaxHeap *heap) {
    int level_start = 0;
    int level = 0;

    printf("  Binary tree (level-order layout):\n");
    while (level_start < heap->size) {
        int level_size = 1 << level;
        int i;
        int indent = (1 << (6 - level)) - 1;
        int spacing = (1 << (7 - level)) - 1;

        printf("    Level %d:", level);
        for (i = 0; i < indent; i++) {
            printf(" ");
        }

        for (i = level_start; i < level_start + level_size && i < heap->size; i++) {
            print_job(heap->data[i]);
            if (i < level_start + level_size - 1 && i < heap->size - 1) {
                int j;
                for (j = 0; j < spacing; j++) {
                    printf(" ");
                }
            }
        }
        printf("\n");

        level_start += level_size;
        level++;
    }
}

static void print_heap_structure(MaxHeap *heap, const char *title) {
    printf("\n%s\n", title);
    print_heap_array(heap);
    print_binary_tree(heap);
}

static int parent(int index) {
    return (index - 1) / 2;
}

static int left_child(int index) {
    return 2 * index + 1;
}

static int right_child(int index) {
    return 2 * index + 2;
}

static void swap_jobs(Job *a, Job *b) {
    Job temp = *a;
    *a = *b;
    *b = temp;
}

static int largest_child_index(MaxHeap *heap, int index) {
    int left = left_child(index);
    int right = right_child(index);
    int largest = index;

    if (left < heap->size && heap->data[left].priority > heap->data[largest].priority) {
        largest = left;
    }
    if (right < heap->size && heap->data[right].priority > heap->data[largest].priority) {
        largest = right;
    }
    return largest;
}

static void heapify_down(MaxHeap *heap, int index, const char *reason) {
    int current = index;

    printf("\n  Heapify-down from index %d (%c, priority %d) %s\n",
           index, heap->data[index].id, heap->data[index].priority, reason);

    while (1) {
        int largest = largest_child_index(heap, current);

        if (largest == current) {
            printf("    Max-heap property satisfied at index %d.\n", current);
            break;
        }

        printf("    Swap index %d ", current);
        print_job(heap->data[current]);
        printf(" with index %d ", largest);
        print_job(heap->data[largest]);
        printf("\n");

        swap_jobs(&heap->data[current], &heap->data[largest]);
        current = largest;
    }
}

static void heapify_up(MaxHeap *heap, int index) {
    int current = index;

    printf("\n  Heapify-up from index %d (%c, priority %d)\n",
           index, heap->data[index].id, heap->data[index].priority);

    while (current > 0) {
        int p = parent(current);

        if (heap->data[current].priority <= heap->data[p].priority) {
            printf("    Parent at index %d ", p);
            print_job(heap->data[p]);
            printf(" is >= child. Stop.\n");
            break;
        }

        printf("    Swap index %d ", current);
        print_job(heap->data[current]);
        printf(" with parent index %d ", p);
        print_job(heap->data[p]);
        printf("\n");

        swap_jobs(&heap->data[current], &heap->data[p]);
        current = p;
    }

    if (current == 0 && index > 0 && heap->data[0].priority >= heap->data[1].priority) {
        printf("    Reached root. Max-heap property restored.\n");
    }
}

static void build_max_heap(MaxHeap *heap) {
    int i;
    int last_non_leaf = heap->size / 2 - 1;

    printf("\n=== Task 2: Convert binary tree to Max-Heap (bottom-up heapify) ===\n");
    printf("Starting from last non-leaf index %d down to root index 0.\n", last_non_leaf);

    for (i = last_non_leaf; i >= 0; i--) {
        printf("\n--- Heapify-down at index %d ---\n", i);
        heapify_down(heap, i, "(build phase)");
    }
}

static void assign_job_ids(MaxHeap *heap, const int priorities[], int count) {
    int i;

    heap->size = count;
    for (i = 0; i < count; i++) {
        heap->data[i].id = (char)('A' + i);
        heap->data[i].priority = priorities[i];
    }
}

static int find_index_by_priority(MaxHeap *heap, int priority) {
    int i;

    for (i = 0; i < heap->size; i++) {
        if (heap->data[i].priority == priority) {
            return i;
        }
    }
    return -1;
}

static void insert_job(MaxHeap *heap, char id, int priority) {
    int index;

    printf("\n=== Task 3: Insert urgent job %c with priority %d ===\n", id, priority);
    printf("Step 1: Append new job at end of array (index %d).\n", heap->size);

    index = heap->size;
    heap->data[index].id = id;
    heap->data[index].priority = priority;
    heap->size++;

    print_heap_structure(heap, "Heap after append (before re-heapify):");

    printf("\nStep 2: Re-adjust using heapify-up (bubble up) to restore max-heap property.");
    heapify_up(heap, index);

    print_heap_structure(heap, "Final heap after insertion:");
}

static int remove_job_by_priority(MaxHeap *heap, int priority) {
    int index;
    Job removed;

    printf("\n=== Task 4: Remove job with priority %d ===\n", priority);

    index = find_index_by_priority(heap, priority);
    if (index < 0) {
        printf("Job with priority %d not found.\n", priority);
        return 0;
    }

    removed = heap->data[index];
    printf("Found job ");
    print_job(removed);
    printf(" at index %d.\n", index);

    if (index == heap->size - 1) {
        heap->size--;
        printf("Removed leaf node. No heapify-down needed.\n");
        print_heap_structure(heap, "Final heap after removal:");
        return 1;
    }

    printf("Step 1: Replace removed node with last element in heap.\n");
    heap->data[index] = heap->data[heap->size - 1];
    heap->size--;

    printf("  Moved ");
    print_job(heap->data[index]);
    printf(" from former last index to index %d.\n", index);

    print_heap_structure(heap, "Heap after replacement (before re-heapify):");

    if (index == 0) {
        printf("\nStep 2: Re-adjust root using heapify-down.");
        heapify_down(heap, 0, "(delete phase)");
    } else {
        int p = parent(index);

        if (heap->data[index].priority > heap->data[p].priority) {
            printf("\nStep 2: Replacement is larger than parent; heapify-up at index %d.", index);
            heapify_up(heap, index);
        } else {
            printf("\nStep 2: Replacement may violate heap downward; heapify-down at index %d.", index);
            heapify_down(heap, index, "(delete phase)");
        }
    }

    print_heap_structure(heap, "Final heap after removal:");
    return 1;
}

static void print_initial_tree_with_ids(MaxHeap *heap) {
    int i;

    printf("\n=== Task 1: Build binary tree from array P ===\n");
    printf("Given priorities P = {42, 17, 93, 28, 65, 81, 54, 60, 99, 73, 88}\n\n");
    printf("=== Job identifiers (Task 1 continued) ===\n");
    printf("Each array index maps to a unique job ID:\n");
    for (i = 0; i < heap->size; i++) {
        printf("  Index %2d -> Job %c, priority %d\n", i, heap->data[i].id, heap->data[i].priority);
    }

    print_heap_structure(heap, "Initial binary tree (array representation, not yet a max-heap):");
}

static void verify_max_heap(MaxHeap *heap, const char *label) {
    int i;
    int valid = 1;

    for (i = 0; i < heap->size; i++) {
        int left = left_child(i);
        int right = right_child(i);

        if (left < heap->size && heap->data[i].priority < heap->data[left].priority) {
            valid = 0;
        }
        if (right < heap->size && heap->data[i].priority < heap->data[right].priority) {
            valid = 0;
        }
    }

    printf("  Max-heap check (%s): %s\n", label, valid ? "VALID" : "INVALID");
}

int main(void) {
    const int priorities[] = {42, 17, 93, 28, 65, 81, 54, 60, 99, 73, 88};
    const int count = (int)(sizeof(priorities) / sizeof(priorities[0]));
    MaxHeap heap;

    g_output_file = fopen(OUTPUT_FILE, "w");
    if (g_output_file == NULL) {
        fprintf(stderr, "Warning: could not create %s\n", OUTPUT_FILE);
    }

    memset(&heap, 0, sizeof(heap));
    assign_job_ids(&heap, priorities, count);

    print_initial_tree_with_ids(&heap);

    build_max_heap(&heap);
    print_heap_structure(&heap, "Max-Heap after build_heap (Task 2 complete):");
    verify_max_heap(&heap, "after build");

    insert_job(&heap, 'L', 100);
    verify_max_heap(&heap, "after insert 100");

    remove_job_by_priority(&heap, 100);
    verify_max_heap(&heap, "after remove 100");

    printf("\nOutput saved to %s\n", OUTPUT_FILE);

    if (g_output_file != NULL) {
        fclose(g_output_file);
        g_output_file = NULL;
    }

    return 0;
}
