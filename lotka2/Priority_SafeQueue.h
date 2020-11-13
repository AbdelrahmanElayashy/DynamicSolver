#ifndef SafeQueue_H
#define SafeQueue_H

#include "MyLibrary.h"
#include <queue>
#include <mutex>
#include <condition_variable>

/*
* Represents a threadsafe Priority_SafeQueue.
*/
template <class T>
class Priority_SafeQueue
{

private:
    /*
    * Represents the Priority_Queue.
    */
    std::priority_queue<T, std::vector<T>, MyLibrary::comp> q;
    /*
    * Represents a mutex.
    */
    mutable std::mutex m;
    /*
    * Represents a condition variable.
    */
    std::condition_variable c;

public:
    /*
    * Represents the constructor of Priority_SafeQueue.
    */
    Priority_SafeQueue(void);
    /*
    * Represents the deconstructor of Priority_SafeQueue.
    */
    ~Priority_SafeQueue(void);

    /*
    * To add an element to the queue.
    */
    void enqueue(T t);

    /*
    * To get the front-element from the queue.
    * If the queue is empty, wait till a element is avaiable.
    */
    T dequeue(void);

    /*
    * To get the size of the queue.
    */
    int get_size();
};
#endif