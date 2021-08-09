#include "structures.h"


int main() {
    std::cout << "To test your own values, or to use these structures yourself, go to structures.cpp and to the data structure you wish to use. You can test the data structures there." << std::endl;
    int userInput;
    while(userInput != 9)
    {
        std::cout << "---------------------------------" << std::endl;
        std::cout << "1: See lists implementations" << std::endl;
        std::cout << "2: See stack implementation" << std::endl;
        std::cout << "3: See queue implementation" << std::endl;
        std::cout << "4: See BST implementation" << std::endl;
        std::cout << "5: See AVL implementation" << std::endl;
        std::cout << "6: See RBT implementation" << std::endl;
        std::cout << "7: See hashtable implementations" << std::endl;
        std::cout << "8: See graph implementations" << std::endl;
        std::cout << "9: Exit program" << std::endl;
        std::cout << "---------------------------------" << std::endl;
        std::cin >> userInput;

        switch(userInput)
        {
            case 1:
                std::cout << "----List Implementation's---" << std::endl;
                myLists();
                break;
            case 2:
                std::cout << "----Stack Implementation---" << std::endl;
                myStack();
                break;
            case 3:
                std::cout << "----Queue Implementation---" << std::endl;
                myQueue();
                break;
            case 4:
                std::cout << "----Binary Search Tree Implementation---" << std::endl;
                myBST();
                break;
            case 5:
                std::cout << "----AVL (Self-Balancing Tree) Implementation---" << std::endl;
                myAVL();
                break;
            case 6:
                std::cout << "----Red Black Tree Implementation---" << std::endl;
                myRBT();
                break;
            case 7:
                std::cout << "----Hashtable Implementation's---" << std::endl;
                myHashtables();
                break;
            case 8:
                std::cout << "----Graph Implementation's---" << std::endl;
                myGraphs();
                break;
            case 9:
                std::cout << "Exiting program..." << std::endl;
                break;
            default:
                std::cout << "Invalid input, try again" << std::endl;
                break;
        }
    }
    return 0;
}