#include "lem-in.h"

void swap(t_path **a, t_path **b)
{
    t_path *temp = *a;
    *a = *b;
    *b = temp;
}

int is_larger_path(t_path *a, t_path *b)
{
    return a->min_left + a->size > b->min_left + b->size;
}

void minHeapify(MinHeap *heap, uint index)
{
    uint smallest = index;
    uint left = 2 * index + 1;
    uint right = 2 * index + 2;

    if (left < heap->size && is_larger_path(heap->paths[smallest], heap->paths[left]))
        smallest = left;
    if (right < heap->size && is_larger_path(heap->paths[smallest], heap->paths[right]))
        smallest = right;

    if (smallest != index)
    {
        swap(&heap->paths[index], &heap->paths[smallest]);
        minHeapify(heap, smallest);
    }
}

void insert(MinHeap *heap, t_path *path, t_data *data)
{
    if (heap->size == heap->capacity)
    {
        heap->capacity *= 2;
        t_path **temp = safe_malloc(sizeof(t_path *) * heap->capacity, data);
        for (uint i = 0; i < heap->size; i++)
            temp[i] = heap->paths[i];
    }

    int i = heap->size++;
    heap->paths[i] = path;

    while (i != 0 && is_larger_path(heap->paths[(i - 1) / 2], heap->paths[i]))
    {
        swap(&heap->paths[i], &heap->paths[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

t_path *extractMin(MinHeap *heap)
{
    if (heap->size <= 0)
        return 0;
    if (heap->size == 1)
        return heap->paths[--heap->size];

    t_path *root = heap->paths[0];
    heap->paths[0] = heap->paths[--heap->size];
    minHeapify(heap, 0);

    return root;
}
