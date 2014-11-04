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
#include "Jacket.h"
#include "Heap.h"

using namespace std;

const int MAX_COLUMN = 20;
const int INFINIT = 99999;
int bestRes;
vector<int> bestComb;
int nodeCounter;
int numOfJackets;
int numOfMachines = 3;

// Create the heap
Heap* myHeap = new Heap();
Jacket job[MAX_COLUMN];

/*This function is called when there is an error*/
void errorHandler(string errorMessage) {
    fclose(stdin);
    cout << "There is an error in computation.. Programm is terminated..." << endl << errorMessage << endl;
    exit(0);
}

/*This fnction is two return greater number*/
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

/* This function is to check required time for a specific combination of machines & jackets*/
Jacket calculateComb(vector<int> idx) {

    Jacket endTime(0, 0, 0);
    for (int i = 0; i < idx.size(); i++) {
        endTime.M1 += job[idx[i]].M1;
        endTime.M2 = findMAx(endTime.M1, endTime.M2) + job[idx[i]].M2;
        endTime.M3 = findMAx(endTime.M2, endTime.M3) + job[idx[i]].M3;
    }
    return endTime;

}

/*This function is finding lowerBound which is the best time it can be possible to complete tasks*/
int lowerBound(Jacket currT, vector<int> list) {
    int totalM1 = 0;
    int totalM2 = 0;
    int totalM3 = 0;
    int bestFinishTimeM23 = 0;
    int bestFinishTimeM3 = 0;

    if (list.size() > 0) {
        bestFinishTimeM23 = INFINIT;
        bestFinishTimeM3 = INFINIT;
    }

    for (int i = 0; i < list.size(); i++) {
        totalM1 += job[list[i]].M1;
        totalM2 += job[list[i]].M2;
        totalM3 += job[list[i]].M3;
        if (job[list[i]].M2 + job[list[i]].M3 < bestFinishTimeM23) {
            bestFinishTimeM23 = job[list[i]].M2 + job[list[i]].M3;
        }
        if (job[list[i]].M3 < bestFinishTimeM3) {
            bestFinishTimeM3 = job[list[i]].M3;
        }
    }
    return findMAx(currT.M1 + totalM1 + bestFinishTimeM23, findMAx(currT.M2 + totalM2 + bestFinishTimeM3, currT.M3 + totalM3));
}

/*
This is the main function to compute branch abd bound
 */
void branchAndBound(vector<int> mySet) {

    //Initial lowerBound for best situation
    int lowerBoundInitial = lowerBound(Jacket(0, 0, 0), mySet);
    int upperBoundGlobal = calculateComb(mySet).M3;

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


        Jacket totalTime = calculateComb(currNode.PassedNodes);

        //if its the final node, then check if it's best result.
        if (currNode.NeedPassNodes.size() == 0) {
            if (totalTime.M3 < bestRes) {
                bestRes = totalTime.M3;
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
            int upperBound = calculateComb(wholeSet).M3;
            //udate global upper bound
            if (upperBound < upperBoundGlobal) {
                upperBoundGlobal = upperBound;
            }
            //get the left boundary of the node
            Jacket newTimeSet = totalTime;
            newTimeSet.M1 += job[currNode.NeedPassNodes[i]].M1;
            newTimeSet.M2 = max(newTimeSet.M1, newTimeSet.M2) + job[currNode.NeedPassNodes[i]].M2;
            newTimeSet.M3 = max(newTimeSet.M2, newTimeSet.M3) + job[currNode.NeedPassNodes[i]].M3;
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
                    cin >> job[i].M1;
                }
                for (int i = 0; i < numOfJackets; i++) {
                    cin >> job[i].M2;
                }
                for (int i = 0; i < numOfJackets; i++) {
                    cin >> job[i].M3;
                }
                fclose(stdin);

                /*Starting of BB algorithm*/
                vector<int> mySet;

                for (int i = 0; i < numOfJackets; i++) {
                    mySet.push_back(i);
                }

                cout << "Started Branch and Bounds computation for " << numOfJackets << " jackets..." << endl;
                bestRes = INFINIT;

                Heap *heap = new Heap();
                //branchAndBound(heap, bestRes, numOfJackets);
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


    }    catch (exception ex) {
        errorHandler((string) ex.what());
    }

    return 0;

}
