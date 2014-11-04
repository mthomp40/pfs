//
//  main.cpp
//  pfs
//
//  Created by Behzad Khalili on 1/11/2014.
//  Copyright (c) 2014 Behzad Khalili. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Node.h"
#include "Heap.h"

using namespace std;

const int MAX_COLUMN = 20;
const int INFINIT = 99999;
int bestRes;
vector<int> bestComb;
int nodeCounter;
int numOfJackets = 4;
int numOfMachines = 3;

class Jacket {
public:

    Jacket() {
    };

    Jacket(vector<int> minutes) {
        this->minutes = minutes;
    };

    int getMinutes(int index) {
        return minutes[index];
    }

    void setMinutes(int index, int value) {
        minutes[index] = value;
    }
private:
    vector<int> minutes;
};

// Create the heap
Heap* myHeap = new Heap();
vector<vector<int> > job = vector<vector<int> >(numOfMachines, vector<int>(numOfJackets, 0));

/*This function is called when there is an error*/
void errorHandler(string errorMessage) {
    fclose(stdin);
    cout << "There is an error in computation.. Programm is terminated..." << endl << errorMessage << endl;
    exit(0);
}

int findMAx(int n1, int n2) {
    if (n1 > n2) {
        return n1;
    } else {
        return n2;
    }
}

/*This function is to return string value*/
template < typename T > string toString(const T& n) {
    ostringstream val;
    val << n;
    return val.str();
}

Jacket getSequenceTime(vector<int> nodes) {
    Jacket jacket(vector<int>(3, 0));
    for (int i = 0; i < nodes.size(); i++) {
        jacket.setMinutes(0, jacket.getMinutes(0) + job[0][nodes[i]]);
        jacket.setMinutes(1, findMAx(jacket.getMinutes(0), jacket.getMinutes(1)) + job[nodes[i]][1]);
        jacket.setMinutes(2, findMAx(jacket.getMinutes(0), jacket.getMinutes(2)) + job[nodes[i]][2]);
    }
    return jacket;
}

/*This function is finding lowerBound which is the best time it can be possible to complete tasks*/
int lowerBound(Jacket currT, vector<int> list) {
    vector<int> totals = vector<int>(numOfMachines, 0);
    int bestFinishTimeM23 = 0;
    int bestFinishTimeM3 = 0;

    if (list.size() > 0) {
        bestFinishTimeM23 = INFINIT;
        bestFinishTimeM3 = INFINIT;
    }

    for (int i = 0; i < list.size(); i++) {
        totals[0] += job[0][list[i]];
        totals[1] += job[1][list[i]];
        totals[2] += job[2][list[i]];
        if (job[1][list[i]] + job[2][list[i]] < bestFinishTimeM23) {
            bestFinishTimeM23 = job[1][list[i]] + job[2][list[i]];
        }
        if (job[2][list[i]] < bestFinishTimeM3) {
            bestFinishTimeM3 = job[2][list[i]];
        }
    }
    return findMAx(currT.getMinutes(0) + totals[0] + bestFinishTimeM23,
            findMAx(currT.getMinutes(1) + totals[1] + bestFinishTimeM3, currT.getMinutes(2) + totals[2]));
}

/*
This is the main function to compute branch abd bound
 */
void branchAndBound(vector<int> mySet) {

    //Initial lowerBound for best situation
    int lowerBoundInitial = lowerBound(Jacket(vector<int>(numOfMachines, 0)), mySet);
    Jacket jacket = getSequenceTime(mySet);
    int upperBoundGlobal = jacket.getMinutes(2);

    /*
     I could change this to a vector and the only difference is in queue we pop from the front. To pop up data from the front with this--> myQueue.erase(myQueue.begin()); but I prefered to keep queue as it is cleaner and tidier
     */


    //myQueue = priority_queue <Node>();



    //myQueue.push(Node(vector<int>(), mySet, lowerBoundInitial));
    myHeap->insert(Node(vector<int>(), mySet, lowerBoundInitial));


    //While there is still
    while (myHeap->size() > 0) {

        //Get the toppest node in the queue and remove the item from queue
        Node currNode = myHeap->deleteNode();
        //Node currNode = myQueue.top();
        //Increase number of nodes
        nodeCounter++;
        //Remove node from queue
        //myQueue.pop();


        Jacket totalTime = getSequenceTime(currNode.PassedNodes);

        //if its the final node, then check if it's best result.
        if (currNode.NeedPassNodes.size() == 0) {
            if (totalTime.getMinutes(2) < bestRes) {
                bestRes = totalTime.getMinutes(2);
                bestComb = currNode.PassedNodes;
            }
            continue;
        }

        //iterate through all possible nodes to go
        for (int i = 0; i < currNode.NeedPassNodes.size(); i++) {
            vector<int> newCurrentSet(currNode.PassedNodes);
            newCurrentSet.push_back(currNode.NeedPassNodes[i]);
            vector<int> newCombinationsLeft(currNode.NeedPassNodes);
            newCombinationsLeft.erase(newCombinationsLeft.begin() + i);

            //get the right boundary of the choosen node
            //calculate the overall time of the current ordered set
            vector<int> wholeSet(currNode.PassedNodes);
            wholeSet.insert(wholeSet.end(), currNode.NeedPassNodes.begin(), currNode.NeedPassNodes.end());
            int upperBound = getSequenceTime(wholeSet).getMinutes(2);
            //udate global upper bound
            if (upperBound < upperBoundGlobal) {
                upperBoundGlobal = upperBound;
            }
            //get the left boundary of the node
            Jacket newTimeSet = totalTime;
            newTimeSet.setMinutes(0, newTimeSet.getMinutes(0)+ job[0][currNode.NeedPassNodes[i]]);
            newTimeSet.setMinutes(1, max(newTimeSet.getMinutes(0), newTimeSet.getMinutes(1)) + job[1][currNode.NeedPassNodes[i]]);
            newTimeSet.setMinutes(2, max(newTimeSet.getMinutes(1), newTimeSet.getMinutes(2)) + job[2][currNode.NeedPassNodes[i]]);
            int lowBound = lowerBound(newTimeSet, newCombinationsLeft);

            //check boundaries, do not go further id lower bound worse then global upper bound
            if (lowBound > upperBoundGlobal) {
                continue;
            }

            //add new node to the query
            myHeap->insert(Node(newCurrentSet, newCombinationsLeft, lowBound));
        }
    }
}

vector<int> branchAndBound(Heap *heap, int best, int n) {
    //cout << heap->size();
    //return best;

    return vector<int>();
}

int main(int argc, const char * argv[]) {

    try {
        if (argc > 1) {

            string filePath = argv[1];

            if (freopen(filePath.c_str(), "r", stdin) != NULL) {
                cin >> numOfJackets;
                for (int i = 0; i < numOfJackets; i++) {
                    cin >> job[0][i];
                }
                for (int i = 0; i < numOfJackets; i++) {
                    cin >> job[1][i];
                }
                for (int i = 0; i < numOfJackets; i++) {
                    cin >> job[2][i];
                }
                fclose(stdin);

                /*Starting of BB algorithm*/
                vector<int> mySet;

                for (int i = 0; i < numOfJackets; i++) {
                    mySet.push_back(i);
                }

                cout << "Started Branch and Bounds computation for " << numOfJackets << " jackets..." << endl;
                bestRes = INFINIT;
                branchAndBound(mySet);
                if (bestRes != INFINIT) {
                    cout << "Branch and Bounds calculation finished" << endl;
                    cout << "Best time: " << bestRes << " minutes" << endl;
                    cout << "Winning combination:";
                    for (int i = 0; i < bestComb.size(); i++) {
                        cout << " " << bestComb[i] + 1;
                    }
                    cout << endl << "Nodes created during the search: " << nodeCounter << endl;
                } else {
                    cout << "Unexpected error occured, solution was not found." << endl;
                }


            } else {
                errorHandler("input file was not found or was inaccessible at " + filePath);
            }

        } else {
            cout << "No input file.Terminating...." << endl;
            exit(0);
        }


    } catch (exception ex) {
        errorHandler((string) ex.what());
    }

    return 0;

}
