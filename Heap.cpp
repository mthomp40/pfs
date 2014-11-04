#include "Heap.h"

bool operator>(Node n1, Node n2) {
    if (n1.LowerBound != n2.LowerBound) {
        return n1.LowerBound > n2.LowerBound;
    }
    return n1.NeedPassNodes.size() > n2.NeedPassNodes.size();
}

Heap::Heap() {
}

Heap::~Heap() {
}

void Heap::insert(Node element) {
    heap.push_back(element);
    heapifyup((int) heap.size() - 1);
}

void Heap::heapifyup(int index) {
    while ((index > 0) && (parent(index) >= 0) && (heap[parent(index)] > heap[index])) {
        Node tmp = heap[parent(index)];
        heap[parent(index)] = heap[index];
        heap[index] = tmp;
        index = parent(index);
    }
}

void Heap::heapifydown(int index) {
    int child = left(index);
    if ((child > 0) && (right(index) > 0) &&
            (heap[child] > heap[right(index)])) {
        child = right(index);
    }
    if (child > 0) {
        Node tmp = heap[index];
        heap[index] = heap[child];
        heap[child] = tmp;
        heapifydown(child);
    }
}

Node Heap::deleteNode() {
    Node min = heap.front();
    heap[0] = heap.at(heap.size() - 1);
    heap.pop_back();
    heapifydown(0);
    return min;
}

int Heap::left(int parent) {
    int i = (parent << 1) + 1;
    return ( i < heap.size()) ? i : -1;

}

int Heap::right(int parent) {
    int i = (parent << 1) + 2;
    return ( i < heap.size()) ? i : -1;
}

int Heap::parent(int child) {
    if (child != 0) {
        int i = (child - 1) >> 1;
        return i;
    }
    return -1;
}
