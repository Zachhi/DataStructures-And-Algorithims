#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H
#include <sstream>
#include <iostream>
template <typename Comparable>
class BinarySearchTree {

private:
    struct Node{
        Comparable data;
        Node *left;
        Node *right;

        Node(): data(), left(nullptr), right(nullptr){}
    };
public:
    Node *root = nullptr;
    
public:
    //default constructor
    BinarySearchTree(): root(nullptr){}

    //copy constructor
    BinarySearchTree(const BinarySearchTree& other) {
        if(other.root == nullptr)
        {
            delete root;
            root = nullptr;
        }
        else
            copyHelper(root, other.root);
    }

    //help for copy contructor and assignment op
    void copyHelper(Node *& lhsRoot, Node *rhsRoot)
    {
        if(rhsRoot == nullptr)
        {
            delete lhsRoot;
            lhsRoot = nullptr;
        }
        else
        {


                lhsRoot = newNode(rhsRoot->data);
                copyHelper(lhsRoot->left, rhsRoot->left);
                copyHelper(lhsRoot->right, rhsRoot->right);
            

        }
    }

    //destructor helper
    void desHelp(Node* node)
    {
        if(node)
        {
            desHelp(node->left);
            desHelp(node->right);
            delete node;
        }
    }
    //destructor
    ~BinarySearchTree()
    {
        desHelp(root);
        //delete root;
    }

    //Assigment operator
    BinarySearchTree& operator=(const BinarySearchTree& other)
    {
        
        if(this!=&other) {
            desHelp(this->root);            
            copyHelper(this->root, other.root);
        }
        return *this;
    }

    Node* newNode(const Comparable& elem)
    {
        Node* node = new Node;
        node->data = elem;
        node->left = node->right = nullptr;
        return node;
    }
    //if tree contains elem, return true. else false
    bool contains(const Comparable& elem) const
    {
        Node *temp;
        temp = root;
        while(temp != nullptr)
        {
            if(temp->data == elem)
                return true;
            else if(temp->data > elem)
                temp = temp->left;
            else
                temp = temp->right;
        }
        return false;
    }

    void insert(const Comparable& elem)
    {
        insertHelper(root, elem);
    }

    void insertHelper(Node* &rt, const Comparable& elem)
    {
        if(rt==nullptr)
        {
            rt = newNode(elem);
            return;
        }
        if(elem > rt->data)
        {
            insertHelper(rt->right, elem);
        }
        else if (elem < rt->data)
            insertHelper(rt->left, elem);
        else
            return;
        
    }

    void remove(const Comparable& elem)
    {
        removeHelper(root, elem);
    }

    Node* removeHelper(Node* rt, const Comparable& elem)
    {
        if(rt == nullptr)
            return rt;
        
        if(elem < rt->data)
            rt->left = removeHelper(rt->left, elem);
        else if(elem > rt->data)
            rt->right = removeHelper(rt->right, elem);

        //This is where we delete the node
        else 
        {
            //if no children
            if(rt->left==nullptr && rt->right==nullptr)
            {
                if(rt == root)
                {
                    root = nullptr;
                    delete rt;
                    return root;
                }
                else
                {
                    delete rt;
                    return nullptr;
                }
            }
            //if no left child
            else if(rt->left == nullptr)
            {
                Node* temp = rt->right;
                if(rt == root)
                {
                    root = temp;
                    delete rt;
                    return temp;
                }
                else
                {
                delete rt;
                return temp;
                }
            }
            //if no right child
            else if(rt->right == nullptr)
            {
                Node* temp = rt->left;
                if(rt == root)
                {
                    root = temp;
                    delete rt;
                    return temp;
                }
                else
                {  
                delete rt;
                return temp;
                }
            }
            //if two children
            Node* temp = find_min_node(rt->right);
            rt->data = temp->data;
            rt->right = removeHelper(rt->right, temp->data);
        }
        return rt;
    }

    const Comparable& find_min() const
    {
        if(root==nullptr)
            throw std::invalid_argument("Tree is empty");

        Node* temp = root;
        while(temp && temp->left != nullptr)
            temp = temp->left;

        //Comparable val = temp->data;
        return temp->data; 
    }

    Node* find_min_node(Node* node)
    {
    Node* temp = node;
    while (temp && temp->left != nullptr)
        temp = temp->left;
    return temp;
    }

    const Comparable& find_max() const
    {
        if(root==nullptr)
            throw std::invalid_argument("Tree is empty");

        Node *temp = root;
        while(temp && temp->right!=nullptr)
            temp = temp->right;
        
        //Comparable val = temp->data;
        return temp->data;
    }

    void print_tree(std::ostream& prnt = std::cout)// const
    {
        if(root==nullptr)
            prnt << "<empty>\n";
        else
            print_help(root, prnt);
    }

    void print_help(Node* rt,  std::ostream& prnt = std::cout, int space = 0, int height = 2)
    {
        //base case
        if(rt == nullptr)
        {
            return;
        }
        space+=height;
        print_help(rt->right, prnt, space, height);
       // prnt << "\n";

        for(int i = height; i < space; i++)
        {
            prnt << ' ';
        }
        prnt << rt->data << "\n";
        print_help(rt->left, prnt, space, height);

    }

    void printTest(Node* root)
    {
        Node* temp = root;
        if(temp!=nullptr)
        {
            printTest(temp->left);
            std::cout << "data: " << temp->data << " right: " << temp->right << " left: " << temp->left;
            printTest(temp->right);
        }
        
    }
};

#endif