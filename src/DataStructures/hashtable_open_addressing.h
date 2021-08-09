#ifndef HASHTABLE_OPEN_ADDRESSING_H
#define HASHTABLE_OPEN_ADDRESSING_H

using namespace std;
#include <functional> 
#include <iostream> 
#include <list>
#include <sstream>
#include <stdexcept> 
#include <vector> 

template <class Key, class Hash=std::hash<Key>>
class HashTableOpenAddressing {
public:
    enum EntryType {ACTIVE, EMPTY, DELETED};

    struct hashInfo
    {
        Key val;
        EntryType info;

        hashInfo(const Key & e = Key{}, EntryType i = EMPTY)
            : val {e}, info {i} {}
        hashInfo(Key && e, EntryType i = EMPTY)
            : val {std::move(e)}, info{i}{}
    };

    vector<hashInfo> table;
    int numCell = 11;
    int numVal = 0;
    float lf = 0;
    float maxLf = 0.5;
public:
    HashTableOpenAddressing()
    {
       vector<hashInfo> table1(11);
       table = table1;
    }
    HashTableOpenAddressing(size_t num)
    {
        vector<hashInfo> table1(num);
        table = table1;
        numCell = num;
    }

    bool is_empty() const// - returns true if the table is empty
    {
        if (numVal < 1)
            return true;
        return false;
    }
    size_t size() const// - returns the number of active values in the table
    {
        return numVal;
    }
    size_t table_size()// const - return the number of cells in the table 
    {
        return numCell;
    }

    void make_empty()
    {
        numVal = 0;
        for (auto & x : table)
            x.info = EMPTY;
    }
    bool insert(const Key& value)// - insert the given lvalue reference into the table, rehashing if the maximum load factor is exceeded, return true if insert was successful (false if item already exists). Don’t overwrite deleted values unless they are equal to value being inserted (i.e. undelete).
    {
        size_t index = position(value);
        if(isActive(index))
            return false;
        
        table[index].val = value;
        table[index].info = ACTIVE;

        numVal ++;
        lf = numVal / (float)numCell;

        //rehash
        if(lf > maxLf)
        {
            rehash();
        }
        
        lf = numVal / (float)numCell;
        return true;
    }
    void rehash()
    {
        vector<hashInfo> oldTable = table;

        int newNumCell = numCell * 2;
        newNumCell = findNextPrime(newNumCell);
        table.resize(newNumCell);
        numCell = newNumCell;
        numVal = 0;
        for(auto & x : table)
            x.info = EMPTY;
            
        for(auto & x: oldTable)
            if(x.info == ACTIVE)
                insert(std::move(x.val));        
    }
    size_t remove(const Key& value)// - remove the specified value from the table, return number of elements removed (0 or 1).  Use lazy deletion.
    {
        size_t index = position(value);
        if(!isActive(index))
            return 0;
        
        table[index].info = DELETED;
        numVal --;
        return 1;
    }

    bool contains(const Key& val) const// - returns Boolean true if the specified value is in the table
    {
        return isActive(position(val));
    }
    size_t position(const Key& value) const// - return the index of the cell that would contain the specified value.  This method handles collision resolution.
    {
        int curr = 1;
        size_t index =  Hash{}(value) % numCell;

        while((table[index].info != EMPTY && table[index].val != value))
            {
                index += 1;
                //curr += 2;
                if(index >= table.size())
                    index -= table.size();

            }
        return index;
    }
    bool isActive(size_t index) const
    {
        return (table[index].info == ACTIVE);
    }




    void print_table(std::ostream&prnt=std::cout) const
    {
        if(is_empty())
            prnt << "<empty>\n";
        else
        {
            prnt << "_______________________" << std::endl;
            prnt << "Index  Values" << std::endl;
            for(int i = 0; i < numCell; i++)
            {
                if(i<10)
                    prnt << i << "       ";
                else if (i < 100)
                    prnt << i << "      ";
                else
                    prnt << i << "     ";
                if(table[i].info == ACTIVE)
                    prnt << table[i].val << std::endl;
                else
                    prnt << std::endl;
            }
        }
    }

    //functions for finding next prime
    int findNextPrime(int n)
    {
        while(!fermatPrime(n, 1))
        {
            n++;
        }
        return n;
    }
    bool fermatPrime(unsigned int n, int amount)
    {
        // edge
        if (n <= 1 || n == 4)  return false;
        if (n <= 3) return true;
        if (n%2 == 0 || n%3 == 0) return false;

        // keep looping
        while (amount>0)
        {
            // pack a rand  
             int a = 2 + rand()%(n-4); 
 
             if (greatestCommonD(n, a) != 1)
                return false;
  
             // Fermat's 
            if (fermatPower(a, n-1, n) != 1)
                return false;
 
            amount--;
        }
        //return true if it makes it here
        return true;
    }
    int greatestCommonD(int a, int b)
    {
        if(a < b)
            return greatestCommonD(b, a);
        else if(a%b == 0)
            return b;
        else return greatestCommonD(b, a%b); 
    }
    int fermatPower(int a, unsigned int n, int x)
    {
        int result = 1;      // Initialize 
        a = a % x;  
 
        while (n > 0)
        {
            if (n & 1)
                result = (result*a) % x;

            n = n/2; 
            a = (a*a) % x;
        }
        return result;
    }
};

#endif  // HASHTABLE_OPEN_ADDRESSING_H