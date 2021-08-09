#ifndef STACK_H
#define STACK_H
#include "array_list.h"
template <typename Object>
class Stack {
   public: 
      Stack() : size(0), arr(){}
   private:
      size_t size;
      ArrayList <Object> arr;
   public:
      
      //rule of three
      Stack(const Stack& other)
      {
         size = other.size;
         arr = other.arr;
      }
      ~Stack()
      {
         
      }
      Stack& operator=(const Stack& other)
      {
         if(this != &other)
         {
            size = other.size;
            this->arr = other.arr;
         }
         return *this;
      }
      void push(const Object& elem)
      {
         arr.insert(size, elem);
         size ++;
      }
      void pop()
      {
         if(size == 0)
            throw std::out_of_range("no elements to pop");
         else
         {
            arr.remove(size-1);
            size--;
         }
      }
      Object& top() {
         if(size == 0)
            throw std::out_of_range("no elements");
         else
            return arr[size-1];      
      }
};

#endif  // STACK_H