#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#include <iostream>


template <typename Object>
class ArrayList {
public: 
    //Constructor
    ArrayList() : capacity(0),Listsize(1), arrList(new Object[1]) {}
    //Overloaded Constructor
    //ArrayList(size_t Asize) : capacity(0), Listsize(Asize), arrList(new Object[Asize]){}
    ArrayList(size_t Asize)
    {
        int s = Asize;
        capacity = 0;
        Listsize = Asize;
        arrList = new Object[s];
    }
    //Copy constructor
    ArrayList(const ArrayList& arr)
    {
        Listsize = arr.Listsize;
        int s = Listsize;
        capacity = arr.capacity;
        delete [] this->arrList;
        arrList = new Object[s];
        for(int i = 0; i < s; i++)
        {
            this->arrList[i] = arr.arrList[i];
        }
    }
private:
    size_t capacity = 1;
    size_t Listsize = 1;
    Object* arrList = new Object[1];
public:

    
    //Destructor
    ~ArrayList() 
    {
        delete [] arrList;
    }
    
    //Assignment operator
    ArrayList& operator=(const ArrayList& arr)
    {
        if(this != &arr)
        {
            capacity = arr.capacity;
            Listsize = arr.Listsize;
            delete[] arrList;
            arrList = new Object[Listsize];
            for(unsigned int i = 0; i < Listsize; i++)
            {
              this->arrList[i] = arr.arrList[i];
            }
        }
        return *this;
    }

    //resize array when needed
    void resize()
    {
        size_t orgSize = Listsize;
        int intOSize = orgSize;
        Listsize*=2;
        Object* tempList = new Object[Listsize];
        for(int i = 0; i < intOSize; i++)
        {
            tempList[i] = arrList[i];
        }
        delete [] arrList;
        arrList = new Object[Listsize];
        for(int i = 0; i < intOSize; i++)
        {
            arrList[i] = tempList[i];
        }
        delete [] tempList;
    }

    //return size
    size_t size() 
    {
        return capacity;
    }

    //utilize []
    Object& operator[](size_t index)
    {
        int intcap = capacity;
        int intindex = index;
        if(-1 < intindex && intindex < intcap)
            return arrList[intindex];
        else
            throw std::out_of_range("index is not valid.");
    }
    
    //insert
    void insert(size_t index, const Object& elem)
    {
        int intindex = index;
        int intcap = capacity;
        if(capacity+1 > Listsize)
            resize();

        if((0<intindex+1) && (intindex <= intcap))
        {
            for(int i = intcap; i >= intindex; i--)
            {
                if(i == intindex)
                    arrList[i]  = elem;
                else
                    arrList[i] = arrList[i-1];
            }
            intcap++;
            capacity++;
        }
        else
            throw std::out_of_range("index is not valid.");      
    }

    //remove
    void remove(size_t index)
    {
        int intindex = index;
        int intcap = capacity;
        if(-1 < intindex && intindex < intcap)
            {
                for(int i = intindex; i < intcap-1; i++)
                {
                    arrList[i] = arrList[i+1];
                }
                capacity--;
                intcap--;
            }
        else
            throw std::out_of_range("index is not valid.");
    }

    //print for testing
    void print()
    {
        std::cout << "The array list contains: ";
        for(int i = 0; i < capacity; i++)
        {
            std::cout << arrList[i] << " ";
        }
        std::cout << std::endl;
    }


};

#endif  // ARRAY_LIST_H