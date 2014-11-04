#ifndef __pfs__Node__
#define __pfs__Node__

#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

class Node
{
public:
    vector<int> NeedPassNodes;
    vector<int> PassedNodes;
    int LowerBound;
    
    Node(vector<int> passedNodes, vector<int> needPassNodes, int lowerBound)
    {
        PassedNodes = passedNodes;
        NeedPassNodes = needPassNodes;
        LowerBound = lowerBound;
    }
};

#endif /* defined(__pfs__Node__) */
