#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H
#include <iostream>


template <typename Object>
class DoublyLinkedList {
    public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), _size(0) {}
    DoublyLinkedList(const DoublyLinkedList& other) 
    {
        if(other.head == nullptr) {
            head = tail = nullptr;
            _size = 0;
        }
        else {
            Node* tmp = other.head;
            int i = 0;
            while(tmp!=nullptr)
            {
                insert(i, tmp->data);
                tmp = tmp->next;
                i++;
            }
        }
        _size = other._size;
    }
    private:
     struct Node
     {
         Node *prev;
         Node *next;
         Object data;
     };
     Node *head, *tail;
     size_t _size = 0;
    public:
    
    ~DoublyLinkedList() 
    {
        Node *temp;
        while(head!=nullptr && head!=tail)
        {
            temp = head;
            head = head->next;
            delete temp;
        }
        delete tail;
    }
    DoublyLinkedList& operator=(const DoublyLinkedList& other) 
    {
        if(this != &other)
        {
         Node *temp;
         while(head!=nullptr && head!=tail)
         {
             temp = head;
             head = head->next;
             delete temp;
         }
         delete tail;
        this->_size = 0;
        head = tail = nullptr;
        if(other.head == nullptr) {
            head = tail = nullptr;
        }
        else {
            Node* tmp = other.head;
            int i = 0;
            while(tmp!=nullptr)
            {
                insert(i, tmp->data);
                tmp = tmp->next;
                i++;
            }
            /*
            Node* cur = other.head;
            size_t size = other._size;
            Node* end = nullptr;
            Node* prv = nullptr;
            for(int q = 0; q < size; q++)
            {
                Node* n = new Node();
                n->data = cur->data;
                if(head==nullptr)
                {
                    head = n;
                    end = head;
                }
                else
                {
                    end->next = n;
                    end = n;
                }
                n->prev = prv;
                prv = n;
                cur = cur->next;
            }
            end->next = nullptr;
            */
        }
        
        }
        _size = other._size;
        return *this;
        
    }
    size_t size() const
    {
        return _size;
    }
    Object& operator[](size_t index)
    {
        int intindex = index;
        int ints = _size;
        if(-1 < intindex && intindex < ints)
        {
            Node* temp = head;
            int count = 0;
            //Object dat;
            while(temp)
            {
                if(count == intindex)
                {
                    break;
                }
                count++;
                temp = temp->next;
            }
            return temp->data;
        }
        else
            throw std::out_of_range("index invalid");
    }
    
    void insert(size_t index, const Object& elem)
    {
        int intindex = index;
        int ints = _size;
        if(-1 < intindex && intindex <= ints)
        {
                //Insert at front
                if(intindex == 0)
                {
                    struct Node* temp;
                    temp = new Node;
                    temp->data = elem;
                    temp->prev = nullptr;
                    temp->next = head;
                    if(_size>0)
                        head->prev = temp;
                    head = temp;
                    if(_size==0)
                        tail = temp;

                    _size++;
                    ints++;
                }
                //Insert at end
                else if (intindex == ints)
                {
                    struct Node* temp;
                    temp = new Node;
                    temp->data = elem;
                    temp->next = nullptr;
                    temp->prev = tail;
                    tail->next = temp;
                    tail = temp;
                    _size++;
                    ints++;
                }
                //Insert at middle
                else 
                {
                    struct Node* temp;
                    temp = new Node;
                    temp->data = elem;
                    Node* curr = head;
                    int count = 0;
                    while(curr)
                    {
                        if(count == intindex-1)
                        {
                            temp->next = curr->next;
                            temp->prev = curr;
                            curr->next->prev = temp;
                            curr->next = temp;
                            break;
                        }
                        curr = curr->next;                       
                        count++;
                    }
                    _size++;
                    ints++;
                }
        }
        else
            throw std::out_of_range("index invalid");
    }
    
    void remove(size_t index)
    {
        int intindex = index;
        int ints = _size;
        if(-1 < intindex && intindex < ints)
        {
            //remove front
            if(intindex == 0)
            {
                // if(head->next!=nullptr)
                // {
                //   Node *temp = head->next;
                //   temp->prev = nullptr;
                //   head = temp;
                // }
                if(head->next!=nullptr)
                {
                    Node* temp = head;
                    head = head->next;
                    delete temp;
                    if(head != nullptr)
                        head->prev = nullptr;
                    
                }
                else
                {
                    delete head;
                    head = nullptr;
                    tail = nullptr;
                }
                // else
                // {
                //     head = tail = nullptr;
                // }
                _size--;
                ints--;
                

            }
            //remove back
            else if(intindex == ints-1)
            {
                // if(intindex == 2)
                // {
                //     tail->prev = head->next;
                // }
                // Node* temp = tail->prev;
                // temp->next = nullptr;
                //tail = temp;
                if(tail)
                {
                    Node *toDel = tail;
                    tail = tail->prev;
                    if(tail)
                        tail->next = nullptr;
                    else
                        head = nullptr;
                    delete toDel;
                }
                _size--;
                ints--;
            }
            //remove middle
            else
            {
                Node* temp = head;
                int count = 0;
                while(temp)
                { 
                      if(count == intindex)
                      {

                        temp->next->prev = temp->prev;
                        temp->prev->next = temp->next;
                        delete temp;
                         _size--;
                          ints--;
                          return;
                      }
                    temp = temp->next;
                    count++; 
                }
            }
        }
        else
            throw std::out_of_range("index invalid");
    }
    void print()
    {
        Node* temp = head;
        while(temp)
        {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }
};

#endif  // DOUBLY_LINKED_LIST_H
