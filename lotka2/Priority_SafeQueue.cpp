#include "Priority_SafeQueue.h"
#include "EquationContainer.h"
#include <iostream>
template class Priority_SafeQueue<EquationContainer>;

    /*
    * Represents the constructor of SafeQueue.
    */
    template<class T>
    Priority_SafeQueue<T>::Priority_SafeQueue(void)
        : q()
        , m()
        , c()
    {}
    /*
    * Represents the deconstructor of SafeQueue.
    */
    template<class T>
    Priority_SafeQueue<T>::~Priority_SafeQueue(void)
    {}

    /*
    * To add an element to the queue.
    */
    template<class T>
    void Priority_SafeQueue<T>::enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_all();
    }

    /* 
    * To get the front-element from the queue.
    * If the queue is empty, wait till a element is avaiable.
    */
    template<class T>
    T Priority_SafeQueue<T>::dequeue(void)
    {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty())
        {
          //  std:: cout << "Waiting" << std::endl;
            // release lock as long as the wait and reaquire it afterwards.
            c.wait(lock);

        }
        T val = q.top();
        q.pop();
        return val;
    }

    /*
    * To get the size of the queue.
    */
    template<class T>
    int Priority_SafeQueue<T>::get_size() {
        return this->q.size();
    }

