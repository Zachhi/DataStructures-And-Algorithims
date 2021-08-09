#ifndef STRUCUTRES_H
#define STRUCTURES_H

#include <iostream>
#include "DataStructures/array_list.h"
#include "DataStructures/doubly_linked_list.h"
#include "DataStructures/stack.h"
#include "DataStructures/queue.h"
#include "DataStructures/binary_search_tree.h"
#include "DataStructures/avl_tree.h"
#include "DataStructures/red_black_tree.h"

#include "DataStructures/hashtable_open_addressing.h"
#include "DataStructures/hashtable_separate_chaining.h"
#include "DataStructures/directedGraph.h"
#include "DataStructures/undirectedGraph.h"

void myLists();     //includes implementation of an arrayList and a doubly linked list
void myStack(); 
void myQueue();
void myBST();
void myAVL();
void myRBT();
void myHashtables();//includes implementation of open addressing and separate chaining hashtables
void myGraphs();    //includes dijkstras and prims minimum spanning tree algorithms

#endif //STRUCTURES_H