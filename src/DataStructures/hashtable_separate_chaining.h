#ifndef HASHTABLE_SEPARATE_CHAINING_H
#define HASHTABLE_SEPARATE_CHAINING_H

using namespace std;
#include <functional> 
#include <iostream> 
#include <list>
#include <sstream>
#include <stdexcept> 
#include <vector> 

template <class Key, class Hash=std::hash<Key>>
class HashTableSeparateChaining {
       vector<vector<Key>> table;
       size_t numBucket;
       size_t numVal = 0;
       float loadFactor = 0;
       float maxLF = 1;
public:

    HashTableSeparateChaining() 
    {
        vector<vector<Key>> table1(11);
        table = table1;
        numBucket = 11;
    }
    HashTableSeparateChaining(size_t num)
    {
        vector<vector<Key>> table1(num);
        table = table1;
        numBucket = num;
    }

    bool is_empty() const
    {
        if(numVal == 0)
            return true;
        return false;
    }
    size_t size() const
    {
        return numVal;
    }

    void print_table(std::ostream&prnt=std::cout) const 
    {
        if(is_empty())
            prnt << "<empty>\n";
        else
        {
        prnt << "_______________________" << std::endl;
        prnt << "Index  Values" << std::endl;
        for(int i = 0; i < numBucket; i++)
        {
            if(i<10)
                prnt << i << "       ";
            else if (i < 100)
                prnt << i << "      ";
            else
                prnt << i << "     ";
            for(Key x : table[i])
                prnt << x << " ---> " ;
            
            prnt << std::endl;
        }
        prnt << "_______________________" << std::endl;
        }
    }

    bool contains(const Key& value) const
    {
        size_t index = Hash{}(value) % numBucket;
        auto & tableIndex = table[index];

        for(auto x : tableIndex)
            if(x == value)
                return true;
        return false;
    }

    void make_empty()
    {
        for(auto & x : table)
            x.clear();
        numVal = 0;
        loadFactor = 0;
    }
    bool insert(const Key& value)
    {   
       size_t index = Hash{}(value) % numBucket;
       auto & indexList = table[index];

       if(contains(value))
            return false;

        indexList.push_back(value);
        numVal++;
        loadFactor = size() / (float)bucket_count();

        if(loadFactor > maxLF)
        {
            int newNumBucket = numBucket*2;
            newNumBucket = findNextPrime(newNumBucket);
            loadFactor = size()/(float)bucket_count();
            rehash(newNumBucket);
        }

        return true;
    
    }
    size_t remove(const Key& value)
    {
        // if(!contains(value))
        //     return false;

        // size_t index = Hash{}(value) % numBucket;
        
        // for(int i = 0; i < table[index].size(); i++)
        // {
        //     if(table[index][i] == value)
        //     {
        //         table[index].erase(table[index].begin()+i);
        //     }
        // }
        size_t index = Hash{}(value) % numBucket;
        auto & tableIndex = table[index];
        int i = 0;
        for(auto x : tableIndex)
        {
            if(x == value)
            {
                table[index].erase(table[index].begin() + i);
                numVal --;
                loadFactor = size()/(float)bucket_count();
                return true;
            }
            i++;
        }
        return false;

    }

    size_t bucket_count() const// - return the number of buckets in the table
    {
        return numBucket;
    }
    size_t bucket_size(size_t index) const// - return the number of values in the specified bucket; throw std::out_of_range if the bucket index is out of bounds of the table.
    {
        if(index >= numBucket || index < 0)
            throw std::out_of_range("invalid index");
        else
        {
        size_t sizeX = table[index].size();
        return sizeX;
        }
    }
    size_t bucket(const Key& value) const// - return the index of the bucket that contains the specified value (or would contain it, if it existed)
    {
        size_t index = Hash{}(value) % numBucket;
        return index;
    }

    float load_factor() const// - return the current load factor of the table
    {
        return loadFactor;
    }
    float max_load_factor() const// - return the current maximum load factor of the table 
    {
        return maxLF;
    }
    void max_load_factor(float newLF)// - set the maximum load factor of the table, forces a rehash if the new maximum is less than the current load factor, throws std::invalid_argument if the input is invalid
    {
        if(newLF <= 0)
            throw std::invalid_argument("invalid");
        else
        {
        maxLF = newLF;
        if(maxLF < loadFactor)
        {
            int newNumBuckets = numBucket*2;
            newNumBuckets = findNextPrime(newNumBuckets);
            loadFactor = size()/(float)newNumBuckets;
            rehash(newNumBuckets);
        }
        }
    }
    void rehash(size_t newBuckets)// - set the number of buckets to the specified value and rehash the table if the total number of buckets has changed. If the new number of buckets would cause the load factor to exceed the maximum load factor, then the new number of buckets is at least size() / max_load_factor().
    {
      //  std::cout << "called with: " << newBuckets << std::endl;
        loadFactor = size()/(float)newBuckets;
        if(loadFactor > maxLF)
        {
            newBuckets = (int)size()/(int)max_load_factor();
            newBuckets = findNextPrime(newBuckets);
            loadFactor = numVal/newBuckets;
        }
        vector<vector<Key>> old = table;
        table.resize(newBuckets);

        for(auto & x : table)
            x.clear();
        numBucket = newBuckets;
        numVal = 0;
        for(auto & x : old)
            for(auto & y : x)
                insert(std::move(y));
        
        //numBucket = newBuckets;
        loadFactor = size()/(float)bucket_count();
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

#endif  // HASHTABLE_SEPARATE_CHAINING_H