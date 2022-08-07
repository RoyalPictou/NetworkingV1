#ifndef THREADWRAPPER__H_
#define THREADWRAPPER__H_
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <assert.h>
#include <chrono>
#include <functional>
class ThreadWrapper
{
    

public:
    // Delete the copy constructor
    ThreadWrapper( const ThreadWrapper & ) = delete;
    // Delete the Assignment opeartor
    ThreadWrapper &operator=( const ThreadWrapper & ) = delete;
    // Parameterized Constructor
    ThreadWrapper( std::function< void() > func );
    // Move Constructor
    ThreadWrapper( ThreadWrapper &&obj );
    // Move Assignment Operator
    ThreadWrapper &operator=( ThreadWrapper &&obj );
    // Destructor
    ~ThreadWrapper();

    private:
    std::thread m_ThreadHandler;
};

#endif