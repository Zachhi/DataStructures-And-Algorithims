#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H
#include <sstream>
#include <iostream>
//FOR HELP
// 1. Every node either red or black
// 2. Every leaf (NULL) is black
// 3. if a node is red, both children are black
// 4. Every simple path from a node to a descendant leaf contains the same number of black nodes.

template <typename Comparable>
class RedBlackTree {
 public:

    enum Color {RED, BLACK};
private:
    struct Node {
        Comparable value;
        Node* left;
        Node* right;
        Node* parent;
        int color;

        // Node(Comparable value)
        // {
        //     this->value = value;
        //     this->left = this->right = this->parent = nullptr;
        //     this->color = RED;
        // }

        Node(): value(), left(nullptr), right(nullptr), parent(nullptr), color(RED){}
        bool isLeft()
        {
            return this == parent->left;
        }
        Node *sibling() 
        {
            if(parent == nullptr)
                return nullptr;
        
            if (isLeft())
                return parent->right;
        
            return parent->left;
        }
        bool hasRed() 
        {
            return (left != nullptr && left->color == RED) || (right != nullptr && right->color == RED);
        }
    };
public:
    Node *root = nullptr;

    //default and rule of 3 here
    RedBlackTree(): root(nullptr){} //- makes an empty tree
    RedBlackTree(const RedBlackTree& other) //- constructs a copy of the given tree
    {
        if(other.root == nullptr)
        {
            delete root;
            root = nullptr;
        }
        else
            copyHelper(root,nullptr, other.root);
    }
    ~RedBlackTree() //- destructs this tree
    {
        desHelp(root);
    }
    RedBlackTree& operator=(const RedBlackTree& other) //- assigns a copy of the given tree
    {
        if(this!=&other)
        {
            desHelp(this->root);
            copyHelper(this->root,nullptr, other.root);
        }
        return *this;
    }

    //copy and des helpers
    void copyHelper(Node *& lhsRoot, Node *newP, Node *rhsRoot)
    {
            if(rhsRoot == nullptr)
            {
               // delete lhsRoot;
                lhsRoot = nullptr;
            }
            else
            {
                lhsRoot = newNode(rhsRoot->value);
                lhsRoot->color = rhsRoot->color;
                lhsRoot->parent = newP;
                copyHelper(lhsRoot->left,lhsRoot, rhsRoot->left);
                copyHelper(lhsRoot->right,lhsRoot, rhsRoot->right);
            }       
    }
    void desHelp(Node* node)
    {
        if(node)
        {
            desHelp(node->left);
            desHelp(node->right);
            delete node;
        }
    }

    //new node
    Node* newNode(const Comparable& elem)
        {
            Node* node = new Node();
            node->value = elem;
            node->left = node->right = node->parent = nullptr;
            node->color = RED;
            return node;
        }

    //print methods
    void print_tree(std::ostream& prnt =std::cout)
    {
        if(root==nullptr)
            std::cout << "<empty>\n";
        else
            printTree(root, prnt);
    }
    void printTree(Node* rt, std::ostream& prnt = std::cout, int space = 0, int height = 2)
    {
        if(rt == nullptr)
            return;
        space+=height;
        printTree(rt->right, prnt, space, height);
        for(int i = height; i < space; i++)
        {
            prnt << ' ';
        }
        if(color(rt) == RED)
            prnt << "R:";
        prnt << rt->value <<   "\n";
        printTree(rt->left, prnt, space, height);

    }

    //easy methods up here (anything not to do with remove and insert)
    int color(const Node* node) const
    {
        if(node == nullptr)
            return BLACK;
        else
            return node->color;
    }
    const Node* get_root() const
    {
        if(root == nullptr)
            return nullptr;
        else
            return root;
    }
    bool contains(const Comparable& elem) const
    {
        Node *temp;
        temp = root;
        while(temp != nullptr)
        {
            if(temp->value == elem)
                return true;
            else if(temp->value > elem)
                temp = temp->left;
            else
                temp = temp->right;
        }
        return false;
    }
    const Comparable& find_min() const
    {
        if(root == nullptr)
            throw std::invalid_argument("Tree is empty");
        
        Node* temp = root;
        while(temp && temp->left != nullptr)
            temp = temp->left;

        return temp->value;
    }
    const Comparable& find_max() const
    {
        if(root==nullptr)
            throw std::invalid_argument("Tree is empty");

        Node *temp = root;
        while(temp && temp->right!=nullptr)
            temp = temp->right;
        
        return temp->value;        
    }

    //insert methods
    Node* BSTInsert(Node* rt, Node* node)
    {
        if(rt == nullptr)   //if empty return node
            return node;
        
        if(node->value < rt->value)   //else recur down tree
        {
            rt->left = BSTInsert(rt->left, node);
            rt->left->parent = rt;
        }
        else if (node->value > rt->value)
        {
            rt->right = BSTInsert(rt->right, node);
            rt->right->parent = rt;
        }

        return rt;
    }
    void fixInsert(Node *& rt, Node *& node)
    {
        Node *nodeParent = nullptr;
        Node *nodeGrand = nullptr;

        while((node != rt) && (node->color != BLACK) && (node->parent->color == RED))
        {
            nodeParent = node->parent;
            nodeGrand = node->parent->parent;

            //CASE A
            if(nodeParent == nodeGrand->left)
            {
                Node *nodeUncle = nodeGrand->right;
                //CASE 1
                if(nodeUncle!=nullptr && nodeUncle->color == RED)
                {
                    nodeGrand->color = RED;
                    nodeParent->color = BLACK;
                    nodeUncle->color = BLACK;
                    node = nodeGrand;
                }
                else
                {
                    //CASE 2
                    if(node == nodeParent->right)
                    {
                        leftRotate(rt, nodeParent);
                        node = nodeParent;
                        nodeParent = node->parent;
                    }
                    //CASE 3
                    rightRotate(rt, nodeGrand);
                    if(nodeParent->color == RED && nodeGrand->color == BLACK)
                    {
                        nodeParent->color = BLACK;
                        nodeGrand->color = RED;
                    }
                    else if (nodeParent->color == BLACK && nodeGrand->color == RED)
                    {
                        nodeParent->color = RED;
                        nodeGrand->color = BLACK;
                    }
                    node = nodeParent;
                }
            }
            //CASE B
            else
            {
                Node *nodeUncle = nodeGrand->left;

                //CASE 1
                if((nodeUncle!=nullptr)&&(nodeUncle->color == RED))
                {
                    nodeGrand->color = RED;
                    nodeParent->color = BLACK;
                    nodeUncle->color = BLACK;
                    node = nodeGrand;
                    
                }
                else
                {
                    //CASE 2
                    if(node == nodeParent->left)
                    {
                        rightRotate(rt, nodeParent);
                        node = nodeParent;
                        nodeParent = node->parent;
                    }

                    //CASE 3
                    leftRotate(rt, nodeGrand);
                    if(nodeParent->color == RED && nodeGrand->color == BLACK)
                    {
                        nodeParent->color = BLACK;
                        nodeGrand->color = RED;
                    }
                    else if (nodeParent->color == BLACK && nodeGrand->color == RED)
                    {
                        nodeParent->color = RED;
                        nodeGrand->color = BLACK;
                    }
                    node = nodeParent;
                }
            }
        }

        rt->color = BLACK;
    }
    void insert(const Comparable& value)
    {
        if(this->contains(value))
            return;
        else
        {
        Node* node = newNode(value);
        root = BSTInsert(root, node);
        fixInsert(root, node);
        }
    }

    //delete methods
    /*
    void remove(const Comparable& value)
    {
        Node *victimNode = getDeletePos(this->root, value);
        if(!victimNode->right)
        {
            Node* node = new Node();
            node->color = BLACK;
            victimNode->right =node;
            victimNode->right->parent = victimNode;
        }
        if(!victimNode->left)
        {
            Node* node = new Node();
            node->color = BLACK;
            victimNode->left =node;
            victimNode->left->parent = victimNode;            
        }
        removeFix(victimNode);
    }
    Node* getDeletePos(Node* rt, Comparable value)
    {
        if(rt==nullptr)
            return rt;
        
        if(value < rt->value)
            return getDeletePos(rt->left, value);
        else if(value > rt->value)
            return getDeletePos(rt->right, value);
        
        if(rt->left == nullptr || rt->right == nullptr)
            return rt;

        Node* temp = getMinValue(rt->right);
        root->value = temp->value;
        return getDeletePos(rt->right, temp->value);
    }
    void removeFix(Node *rt)
    {
        if(rt==nullptr)
            return;
        if(rt==root)
            {
                root = nullptr;
                return;
            }

        //if node to be deleted is red, or if either of its children are red
        if(rt->color == RED || rt->left->color == RED || rt->right->color == RED)
        {
            Node *child =rt->left != nullptr ? rt->left : rt->right;    //node to be deleted will only have child. figure out which

            //if node is a left child
            if (rt == rt->parent->left)
            {
                rt->parent->left = child;
                if(child!=nullptr)
                {
                    child->parent = rt->parent;
                    child->color = BLACK;
                }
                delete (rt);
            }
            else
            {
                rt->parent->right = child;
                if(child!=nullptr)
                {
                    child->parent = rt->parent;
                    child->color = BLACK;
                }
                delete (rt);
            }
        }
        //all 3 are black
        else
        {
            Node *sibling = nullptr;
            Node *parent = nullptr;
            Node *temp = rt;
            temp->Dblack = true;
            while(temp!=root && temp->Dblack)
            {
                parent = temp->parent;

                if(temp == parent->left)            //temp is left child of parent
                {
                    sibling = parent->right;

                    if(!sibling->right)
                    {
                        Node* node = new Node();
                        node->color = BLACK;
                        sibling->right = node;
                        sibling->right->parent = sibling;
                    }
                    if(!sibling->left)
                    {
                        Node* node = new Node();
                        node->color = BLACK;  
                        sibling->left = node;
                        sibling->left->parent = sibling;                     
                    }

                    if(sibling->color == RED) //sibling is red
                    {
                        sibling->color = BLACK;
                        parent->color = RED;
                        leftRotate(parent);
                    }
                    else    //sibling is black
                    {
                        if(sibling->left->color == BLACK && sibling->right->color == BLACK)
                        {
                            sibling->color = RED;
                            if(parent->color == RED)
                                parent->color = BLACK;
                            else
                                parent->Dblack = true;
                            temp = parent;
                        }
                        else
                        {
                            if(sibling->right->color == BLACK)
                            {
                                sibling->left->color = BLACK;
                                sibling->color = RED;
                                rightRotate(sibling);
                                sibling = parent->right;
                            }
                            sibling->color = parent->color;
                            sibling->Dblack = parent->Dblack;
                            parent->color = BLACK;
                            parent->Dblack = false;

                        }
                    }
                }
            }
        }
    }
*/


    void swapValues(Node *u, Node *v)
    {
        Comparable useless;
        useless = u->value;
        u->value = v->value;
        v->value = useless;
    }
    void remove(const Comparable& value)
    {
        if(root == nullptr)
            return;
        
        Node *x = search(value);

        if(x->value != value)
            return;
        
        removeHelp(x);
    }
    void removeHelp(Node *v)
    {
        Node *u = replacer(v);

        bool dbBlack = ((u == nullptr || u->color == BLACK) && (v->color == BLACK));
        Node *parent = v->parent;

        if(u == nullptr)
        {
            if(v == root)
                root = nullptr;
            else
            {
                if(dbBlack)
                {
                    dbHelp(v);
                }
                else
                {
                    if(v->sibling() != nullptr)
                        v->sibling()->color = RED;
                }

                if(v->isLeft())
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
            }
            delete v;
            return;
        }

        //v has one child
        if (v->left == nullptr || v->right == nullptr)
        {
            if(v == root)
            {
                v->value = u->value;
                v->left = v->right = nullptr;
                delete u;
            }
            else
            {
                if(v->isLeft())
                    parent->left = u;
                else
                    parent->right = u;
                
                delete v;
                u->parent = parent;
                if(dbBlack)
                    dbHelp(u);
                else
                    u->color = BLACK;
            }
            return;
        }

        //v has 2 children
        swapValues(u, v);
        removeHelp(u);
    }
    Node *replacer(Node *x)
    {
        if(x->left != nullptr && x->right != nullptr)
            return successor(x->right);
        
        if(x->left == nullptr && x->right == nullptr)
            return nullptr;
        
        if(x->left != nullptr)
            return x->left;
        else
            return x->right;
    }
    Node *successor(Node *x)
    {
        Node *temp = x;

        while(temp->left != nullptr)
            temp = temp->left;
        
        return temp;
    }
    void dbHelp(Node *rt)
    {
        if(rt == root)
            return;
        
        Node *sibling = rt->sibling();
        Node *parent = rt->parent;

        if(sibling == nullptr)
        {
            dbHelp(parent);
        }
        else
        {
            if(sibling->color == RED)
            {
                parent->color = RED;
                sibling->color = BLACK;
                if(sibling->isLeft())
                    rightRotate(rt, parent);
                else
                    leftRotate(rt, parent);
                
                dbHelp(rt);
            }
            else
            {
                if(sibling->hasRed())
                {
                    if(sibling->left != nullptr && sibling->left->color == RED)
                    {
                        if(sibling->isLeft())
                        {
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            rightRotate(rt, parent);
                        }
                        else
                        {
                            sibling->left->color = parent->color;
                            rightRotate(rt, sibling);
                            leftRotate(rt, parent);
                        }
                    }
                    else
                    {
                        if(sibling->isLeft())
                        {
                            sibling->right->color = parent->color;
                            leftRotate(rt, sibling);
                            rightRotate(rt, parent);
                        }
                        else
                        {
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            leftRotate(rt, parent);
                        }
                    }
                    parent->color = BLACK;
                }
                else
                {
                    sibling->color = RED;
                    if(parent->color == BLACK)
                        dbHelp(parent);
                    else
                        parent->color = BLACK;
                }
            }
        }
    }

    //rotations
    void leftRotate(Node *& rt, Node *& node)
    {
        Node *nodeRight = node->right;
        node->right = nodeRight->left;

        if(node->right != nullptr)
            node->right->parent = node;

        nodeRight->parent = node->parent;

        if(node->parent == nullptr)
            rt = nodeRight;
        else if (node == node->parent->left)
            node->parent->left = nodeRight;
        else
            node->parent->right = nodeRight;
        
        nodeRight->left = node;
        node->parent = nodeRight;
    }
    void rightRotate(Node *& rt, Node *& node)
    {
        Node *nodeLeft = node->left;
        node->left = nodeLeft->right;

        if(node->left !=nullptr)
            node->left->parent = node;
        
        nodeLeft->parent = node->parent;

        if(node->parent == nullptr)
            rt = nodeLeft;
        else if (node == node->parent->left)
            node->parent->left = nodeLeft;
        else
            node->parent->right = nodeLeft;

        nodeLeft->right = node;
        node->parent = nodeLeft;
    }  

    //getMinValue
    Node* getMinValue(Node *rt)
    {
        if(rt==nullptr)
            return nullptr;
        else if (rt->left == nullptr)
            return rt;
        else
            return getMinValue(rt->left);
    }

    //search
    Node *search(Comparable n)
    {
        Node *temp = root;
        while (temp != nullptr) 
        {
            if (n < temp->value)
             {
                if (temp->left == nullptr)
                    break;
                else
                    temp = temp->left;
             }     
            else if (n == temp->value) 
                break;
            else 
            {
                if (temp->right == nullptr)
                 break;
                else
                 temp = temp->right;
            }
        }
 
         return temp;
    }
};

#endif  // RED_BLACK_TREE_H

