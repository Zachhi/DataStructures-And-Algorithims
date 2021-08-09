#ifndef QUEUE_H
#define QUEUE_H
#include "array_list.h"
template <typename Object>
class Queue {
    public:
        Queue(): _size(0), arr() {}
        Queue(const Queue& other)
        {
            this->_size = other._size;
            this->arr = other.arr;
        }
    private:
          size_t _size;
          ArrayList <Object> arr;
    public:
        
        //rule of three

        ~Queue()
        {

        }
        Queue& operator=(const Queue& other)
        {
         if(this != &other)
         {
            _size = other._size;
            arr = other.arr;
         }
         return *this;
        }


        void enqueue(const Object& elem)
        {
            arr.insert(_size, elem);
            _size++;
        }
        Object dequeue() {
            if (_size == 0)
                throw std::out_of_range("Queue is empty");
            else
            {
                Object elem = arr[0];
                arr.remove(0);
                _size--;
                return elem;
            }
        }
        size_t size() const 
        {
            return _size;
        }
        const Object& front() {
            if(_size == 0)
                throw std::out_of_range("Queue is empty");
            else
            {
                return arr[0];
            }
        }
};

#endif  // QUEUE_H