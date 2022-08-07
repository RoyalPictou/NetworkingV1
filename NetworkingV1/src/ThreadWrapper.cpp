#include "ThreadWrapper.h"
#include <functional>
/*
 * A class that has thread object as member variable
 */

// Parameterized Constructor
ThreadWrapper::ThreadWrapper( std::function< void() > func )
    : m_ThreadHandler( func )
{
}
// Move Constructor
ThreadWrapper::ThreadWrapper( ThreadWrapper &&obj )
    : m_ThreadHandler( std::move( obj.m_ThreadHandler ) )
{
    std::cout << "Move Constructor is called" << std::endl;
}
// Move Assignment Operator
ThreadWrapper &
ThreadWrapper::operator=( ThreadWrapper &&obj )
{
    std::cout << "Move Assignment is called" << std::endl;
    if ( m_ThreadHandler.joinable() )

    {

        m_ThreadHandler.join();
    }
    m_ThreadHandler = std::move( obj.m_ThreadHandler );
    return *this;
}
// Destructor
ThreadWrapper::~ThreadWrapper()
{
    if ( m_ThreadHandler.joinable() )
    {
        m_ThreadHandler.join();
    }
        
}