/*
 * Question 2: Airport Security Max-Heap Priority Queue
 *
 * Passengers are prioritized by risk score using an array-based max-heap.
 * Demonstrates build, priority queue extraction, insert (98), and remove (98).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_PASSENGERS 64
#define NAME_LEN 32
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
    char name[NAME_LEN];
    int risk_score;
} Passenger;

typedef struct {
    Passenger data[MAX_PASSENGERS];
    int size;
} MaxHeap;

static void print_passenger(Passenger p) {
    printf("%s(%d)", p.name, p.risk_score);
}

static void print_heap_array(MaxHeap *heap) {
    int i;

    printf("  Array (level-order): [");
    for (i = 0; i < heap->size; i++) {
        print_passenger(heap->data[i]);
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
            print_passenger(heap->data[i]);
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

static void swap_passengers(Passenger *a, Passenger *b) {
    Passenger temp = *a;
    *a = *b;
    *b = temp;
}

static int largest_child_index(MaxHeap *heap, int index) {
    int left = left_child(index);
    int right = right_child(index);
    int largest = index;

    if (left < heap->size &&
        heap->data[left].risk_score > heap->data[largest].risk_score) {
        largest = left;
    }
    if (right < heap->size &&
        heap->data[right].risk_score > heap->data[largest].risk_score) {
        largest = right;
    }
    return largest;
}

static void heapify_down_verbose(MaxHeap *heap, int index, const char *reason) {
    int current = index;

    printf("\n  Heapify-down from index %d (%s, risk %d) %s\n",
           index, heap->data[index].name, heap->data[index].risk_score, reason);

    while (1) {
        int largest = largest_child_index(heap, current);

        if (largest == current) {
            printf("    Max-heap property satisfied at index %d.\n", current);
            break;
        }

        printf("    Swap index %d ", current);
        print_passenger(heap->data[current]);
        printf(" with index %d ", largest);
        print_passenger(heap->data[largest]);
        printf("\n");

        swap_passengers(&heap->data[current], &heap->data[largest]);
        current = largest;
    }
}

static void heapify_down(MaxHeap *heap, int index) {
    int current = index;

    while (1) {
        int largest = largest_child_index(heap, current);

        if (largest == current) {
            break;
        }

        swap_passengers(&heap->data[current], &heap->data[largest]);
        current = largest;
    }
}

static void heapify_up(MaxHeap *heap, int index) {
    int current = index;

    printf("\n  Heapify-up from index %d (%s, risk %d)\n",
           index, heap->data[index].name, heap->data[index].risk_score);

    while (current > 0) {
        int p = parent(current);

        if (heap->data[current].risk_score <= heap->data[p].risk_score) {
            printf("    Parent at index %d ", p);
            print_passenger(heap->data[p]);
            printf(" has risk >= child. Stop.\n");
            break;
        }

        printf("    Swap index %d ", current);
        print_passenger(heap->data[current]);
        printf(" with parent index %d ", p);
        print_passenger(heap->data[p]);
        printf("\n");

        swap_passengers(&heap->data[current], &heap->data[p]);
        current = p;
    }

    if (current == 0 && index > 0) {
        printf("    Reached root. Max-heap property restored.\n");
    }
}

static void build_max_heap(MaxHeap *heap) {
    int i;
    int last_non_leaf = heap->size / 2 - 1;

    printf("\n=== Task 1: Build Max-Heap from passenger list ===\n");
    printf("Bottom-up heapify from last non-leaf index %d down to root.\n", last_non_leaf);

    for (i = last_non_leaf; i >= 0; i--) {
        printf("\n--- Heapify-down at index %d ---\n", i);
        heapify_down_verbose(heap, i, "(build phase)");
    }
}

static void init_heap_from_passengers(MaxHeap *heap,
                                      const char *names[],
                                      const int scores[],
                                      int count) {
    int i;

    heap->size = count;
    for (i = 0; i < count; i++) {
        strncpy(heap->data[i].name, names[i], NAME_LEN - 1);
        heap->data[i].name[NAME_LEN - 1] = '\0';
        heap->data[i].risk_score = scores[i];
    }
}

static MaxHeap copy_heap(const MaxHeap *source) {
    MaxHeap copy;

    copy.size = source->size;
    memcpy(copy.data, source->data, (size_t)source->size * sizeof(Passenger));
    return copy;
}

static int extract_max(MaxHeap *heap, Passenger *out) {
    if (heap->size == 0) {
        return 0;
    }

    *out = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    if (heap->size > 0) {
        heapify_down(heap, 0);
    }

    return 1;
}

static void extract_priority_queue(MaxHeap *original) {
    MaxHeap working = copy_heap(original);
    int rank = 1;
    Passenger next;

    printf("\n=== Task 2: Extract priority queue (screening order) ===\n");
    printf("Each extract-max removes the root and applies heapify-down on the copy.\n");
    printf("Screening order (highest risk first):\n\n");

    while (extract_max(&working, &next)) {
        printf("  %d. ", rank++);
        print_passenger(next);
        printf("\n");
    }

    printf("\nScreening order: highest risk score processed first.\n");
    print_heap_structure(original, "Original heap unchanged after extraction demo:");
}

static int find_index_by_risk_score(MaxHeap *heap, int risk_score) {
    int i;

    for (i = 0; i < heap->size; i++) {
        if (heap->data[i].risk_score == risk_score) {
            return i;
        }
    }
    return -1;
}

static void insert_passenger(MaxHeap *heap, const char *name, int risk_score) {
    int index;

    printf("\n=== Task 3: Insert new passenger %s with risk score %d ===\n", name, risk_score);
    printf("Step 1: Append passenger at end of array (index %d).\n", heap->size);

    index = heap->size;
    strncpy(heap->data[index].name, name, NAME_LEN - 1);
    heap->data[index].name[NAME_LEN - 1] = '\0';
    heap->data[index].risk_score = risk_score;
    heap->size++;

    print_heap_structure(heap, "Heap after append (before re-heapify):");

    printf("\nStep 2: Re-adjust using heapify-up to preserve max-heap property.");
    heapify_up(heap, index);

    print_heap_structure(heap, "Final heap after insertion:");
}

static int remove_passenger_by_risk_score(MaxHeap *heap, int risk_score) {
    int index;
    Passenger removed;

    printf("\n=== Task 4: Remove passenger with risk score %d ===\n", risk_score);

    index = find_index_by_risk_score(heap, risk_score);
    if (index < 0) {
        printf("Passenger with risk score %d not found.\n", risk_score);
        return 0;
    }

    removed = heap->data[index];
    printf("Found passenger ");
    print_passenger(removed);
    printf(" at index %d.\n", index);

    if (index == heap->size - 1) {
        heap->size--;
        printf("Removed leaf node. No re-heapify needed.\n");
        print_heap_structure(heap, "Final heap after removal:");
        return 1;
    }

    printf("Step 1: Replace removed node with last element in heap.\n");
    heap->data[index] = heap->data[heap->size - 1];
    heap->size--;

    printf("  Moved ");
    print_passenger(heap->data[index]);
    printf(" from former last index to index %d.\n", index);

    print_heap_structure(heap, "Heap after replacement (before re-heapify):");

    if (index == 0) {
        printf("\nStep 2: Re-adjust root using heapify-down.");
        heapify_down_verbose(heap, 0, "(delete phase)");
    } else {
        int p = parent(index);

        if (heap->data[index].risk_score > heap->data[p].risk_score) {
            printf("\nStep 2: Replacement exceeds parent risk; heapify-up at index %d.", index);
            heapify_up(heap, index);
        } else {
            printf("\nStep 2: Replacement may violate heap downward; heapify-down at index %d.", index);
            heapify_down_verbose(heap, index, "(delete phase)");
        }
    }

    print_heap_structure(heap, "Final heap after removal:");
    return 1;
}

static void print_passenger_list(const char *names[], const int scores[], int count) {
    int i;

    printf("\n=== Initial passenger records ===\n");
    for (i = 0; i < count; i++) {
        printf("  %s -> risk score %d\n", names[i], scores[i]);
    }
}

static void verify_max_heap(MaxHeap *heap, const char *label) {
    int i;
    int valid = 1;

    for (i = 0; i < heap->size; i++) {
        int left = left_child(i);
        int right = right_child(i);

        if (left < heap->size &&
            heap->data[i].risk_score < heap->data[left].risk_score) {
            valid = 0;
        }
        if (right < heap->size &&
            heap->data[i].risk_score < heap->data[right].risk_score) {
            valid = 0;
        }
    }

    printf("  Max-heap check (%s): %s\n", label, valid ? "VALID" : "INVALID");
}

int main(void) {
    const char *names[] = {"Alice", "Brian", "Chen", "Fatima", "Noah", "Sofia"};
    const int scores[] = {72, 45, 91, 63, 88, 55};
    const int count = (int)(sizeof(scores) / sizeof(scores[0]));
    MaxHeap heap;

    g_output_file = fopen(OUTPUT_FILE, "w");
    if (g_output_file == NULL) {
        fprintf(stderr, "Warning: could not create %s\n", OUTPUT_FILE);
    }

    memset(&heap, 0, sizeof(heap));
    init_heap_from_passengers(&heap, names, scores, count);

    print_passenger_list(names, scores, count);
    print_heap_structure(&heap, "Initial array layout (before max-heap build):");

    build_max_heap(&heap);
    print_heap_structure(&heap, "Max-Heap after build (Task 1 complete):");
    verify_max_heap(&heap, "after build");

    extract_priority_queue(&heap);
    verify_max_heap(&heap, "after priority queue extraction demo");

    insert_passenger(&heap, "David", 98);
    verify_max_heap(&heap, "after insert 98");

    remove_passenger_by_risk_score(&heap, 98);
    verify_max_heap(&heap, "after remove 98");

    printf("\nOutput saved to %s\n", OUTPUT_FILE);

    if (g_output_file != NULL) {
        fclose(g_output_file);
        g_output_file = NULL;
    }

    return 0;
}
