//
//  Heap.h
//  pfs
//
//  Created by Behzad Khalili on 2/11/2014.
//  Copyright (c) 2014 Behzad Khalili. All rights reserved.
//

#ifndef __pfs__Heap__
#define __pfs__Heap__

#include <iostream>
#include <vector>
#include <iterator>
#include "Node.h"

using namespace std;

class Heap {
public:
    Heap();
    ~Heap();
    void insert(Node element);
    Node deleteNode();
    void print();
    int size() { return (int) heap.size(); }
private:
    int left(int parent);
    int right(int parent);
    int parent(int child);
    void heapifyup(int index);
    void heapifydown(int index);
private:
    vector<Node> heap;
};



#endif /* defined(__pfs__Heap__) */
