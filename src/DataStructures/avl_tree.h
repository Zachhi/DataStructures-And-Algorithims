#ifndef AVL_TREE_H
#define AVL_TREE_H
#include <sstream>
#include <iostream>
template <typename Comparable>
class AVLTree {
    private:
        struct Node
        {
            Comparable data;
            int height;
            Node *left;
            Node *right;

            Node(): data(), height(0), left(nullptr), right(nullptr){}
        };

    
    public: 
        Node *root;
        AVLTree(): root(nullptr) {}
        AVLTree(const AVLTree& other)
        {
        if(other.root == nullptr)
        {
            delete root;
            root = nullptr;
        }
        else
            copyHelper(root, other.root);
        }
        void copyHelper(Node *& lhsRoot, Node *rhsRoot)
        {
            if(rhsRoot == nullptr)
            {
                delete lhsRoot;
                lhsRoot == nullptr;
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
        ~AVLTree()
        {
             desHelp(root);
        }
        AVLTree& operator=(const AVLTree& other)
        {
        if(this!=&other) {
            desHelp(this->root);
            copyHelper(this->root, other.root);
        }
        return *this;
        }
        //simple height access
        int height(Node *node)
        {
            if (node == nullptr)
                return -1;
            return node->height;
        }

        //simple max checking
        int max(int a, int b)
        {
            if(a > b)
                return a;
            else
                return b;
        }

        //to create a new node simply
        Node* newNode(const Comparable& elem)
        {
            Node* node = new Node();
            node->data = elem;
            node->left = node->right = nullptr;
            node->height = 0;
            return node;
        }

        //get the balance factor
        int getBal(Node* node)
        {
            if(node == nullptr)
                return 0;
            return height(node->left) - height(node->right);
        }
        //balance function
        void balanceTree(Node* &node)
        {
            if(node == nullptr)
                return;

            if(height(node->left) - height(node->right) > 1) //if left subtree is greater than right subtree by more than 1
                if(height(node->left->left) >= height(node->left->right))
                    rotateRight(node);
                else
                    leftRightRotation(node);
            else
                if(height(node->right)-height(node->left) > 1)
                    if(height(node->right->right) >= height(node->right->left))
                        rotateLeft(node);
                    else
                        rightLeftRotation(node);
            
            node->height = max(height(node->left), height(node->right)) + 1;
        }
        //insert functions
        void insert(const Comparable& elem)
        {
            insertHelper(elem, root);
        }    
        void insertHelper(const Comparable& elem, Node* &rt)
        {
            if(rt == nullptr)
            {
                rt = newNode(elem);
            }
            else if(elem < rt->data)
                insertHelper(elem, rt->left);
            else if(rt->data < elem)
                insertHelper(elem, rt->right);
            balanceTree(rt);
        }   
        //removal
        void remove(const Comparable& elem)
        {
            removeHelper(elem, root);
        }
        void removeHelper(const Comparable& elem, Node* &x)
        {
            if(x==nullptr)
                return;                                             //no item, do nothing
            if(elem < x->data)                                      //same as insert here
                removeHelper(elem, x->left);
            else if(x->data < elem)
                removeHelper(elem, x->right);
            else if (x->left != nullptr && x->right != nullptr)     //if not two cases above, this is the node to remove
            {
                x->data = minValueNode(x->right)->data;                 //for 2 children
                removeHelper(x->data, x->right);     
            }
            else
            {
                Node* oldie = x;
                x= (x->left != nullptr) ? x->left : x->right;       //check if left or right child is missing
                delete oldie;

            }
            balanceTree(x);                                         //balance when done
        }
        //rotations
        void rotateRight(Node* &x2) //rotate with left child
        {
            Node* x1 = x2->left;
            x2->left = x1->right;
            x1->right = x2;
            x2->height = max(height(x2->left), height(x2->right)) + 1;
            x1->height = max(height(x1->left), x2->height) + 1;
            x2 = x1;
        }
        void rotateLeft(Node* &x2) //rotate with right child
        {
            Node* x1 = x2->right;
            x2->right = x1->left;
            x1->left = x2;
            x2->height = max(height(x2->right), height(x2->left)) + 1;
            x1->height = max(height(x1->right), x2->height) + 1;
            x2 = x1;
        }
        void leftRightRotation(Node* &x3)
        {
            rotateLeft(x3->left);
            rotateRight(x3);
        }      
        void rightLeftRotation(Node* &x3)
        {
            rotateRight(x3->right);
            rotateLeft(x3);
        }
/*
        //insertion functions
       // void insert(const Comparable& elem)
        //{
          //  root = insertHelper(root, elem);
        //}

        Node* insertHelper(Node* rt, const Comparable& elem)
        {
            //1) basic bst insertion
            //base
            if(rt == nullptr)
            {
                return newNode(elem);
            }
            
            //go left or right
            if(elem < rt->data)
                rt->left = insertHelper(rt->left, elem);
            else if ( elem > rt->data)
                rt->right = insertHelper(rt->right, elem);
            else
                return rt;
            
            //2) update height
            rt->height = 1 + max(height(rt->left), height(rt->right));

            //3) get balance to check if it became unbalanced
            int bal = getBal(rt);

            //now, if unbalanced check if conditions

            //right rotate
            if (bal > 1 && elem < rt->left->data)
            {
                return rightRotation(rt);
            }
            //left rotate
            if (bal < -1 && elem > rt->right->data)
            {
                return leftRotation(rt);
            }

            //left right
            if (bal > 1 && elem > rt->left->data)
            {
                return leftRightRotation(rt);
            }

            //right left
            if(bal < -1 && elem < rt->right->data)
            {
                return rightLeftRotation(rt);
            }

            return rt;
        }

        //rotations
        Node* rightRotation(Node* y) //single rotate with left child
        {
            Node* x = y->left;
            Node* z = x->right;

            x->right = y;
            y->left = z;

            y->height = max(height(y->left), height(y->right)) + 1;
            x->height = max(height(x->left), height(x->right)) + 1;
            return x;
        }

        Node* leftRotation(Node* x) //single rotate with right child
        {
            Node *y = x->right;
            Node *z = y->left;
 
            // Perform rotation
            y->left = x;
            x->right = z;
 
            // Update heights
            x->height = max(height(x->left), height(x->right)) + 1;
            y->height = max(height(y->left), height(y->right)) + 1;
 
            // Return new root
            return y;
        }

        Node* leftRightRotation(Node* x) 
        {
            x->left = leftRotation(x->left);
            return rightRotation(x);
        }

        Node* rightLeftRotation(Node* x)
        {
            x->right = rightRotation(x->right);
            return leftRotation(x);
        }
*/

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

        Node* minValueNode(Node* node) 
        { 
         Node* temp = node; 
         while (temp->left != nullptr) 
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
/*
        void remove(const Comparable& elem)
        {
            root = removerHelper(root, elem);
        }

        Node* removerHelper(Node* rt, const Comparable& elem)
        {
            if(rt == nullptr)
                return rt;
            
            if(elem < rt->data)
                rt->left = removerHelper(rt->left, elem);
            else if (elem > rt->data)
                rt->right = removerHelper(rt->right, elem);
            else //node to delete
            {
                if((rt->left == nullptr) || (rt->right == nullptr))
                {
                    Node *temp = rt->left ? rt->left : rt->right;
                    if(temp == nullptr)
                    {
                        temp = rt;
                        rt = nullptr;
                    }
                    else
                    {
                        *root = *temp;
                    }
                    free(temp);
                }
                else
                {
                    Node* temp = minValueNode(rt->right);
                    rt->data = temp->data;
                    rt->right = removerHelper(rt->right, temp->data);
                }
            }
            if(rt == nullptr)
                return rt;
            
            //update height
            rt->height = 1 + max(height(rt->left), height(rt->right));

            int bal = getBal(rt);

            if(bal > 1 && getBal(rt->left) >= 0)
                return rightRotation(rt);
            if(bal < -1 && getBal(rt->right) <= 0)
                return leftRotation(rt);
            if(bal > 1 && getBal(rt->left) < 0)
            {
                rt->left = leftRotation(rt->left);
                return rightRotation(rt);
            }
            if(bal < -1 && getBal(rt->right) > 0)
            {
                rt->right = rightRotation(rt->right);
                return leftRotation(rt);
            }

            return rt;
        }
*/
        void print_tree(std::ostream& prnt =std::cout)// const
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
};

#endif