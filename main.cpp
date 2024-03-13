// Exercise

// Implement a simple version of C++'s std::set container, specially designed for
// std::strings. In this simple version you can add strings, check if
// a certain string is already added to the container, and remove strings.
// You can use C++11 features for the implementation.

// It is up to you how you want to deal with the internal storage, but it
// is not allowed to just write a wrapper around std::set.


// Methods:

// The methods that should be available for this container are:

// - bool add(const std::string &data);

// With this method you can add a string. The return value indicates if the string was already
// added to the set or not.

// - bool remove(const std::string &data);

// With this method you can remove a string. The return value indicates if the string
// was indeed in the set.

// - bool contains(const std::string &data);

// With this method you check if the string is in the container.

// The code below should compile:

#include <iostream>
#include "string_set.h"

/**
 *  The main function
 */
int main()
{
    // create your set
    StringSet set;
    
    // add some data
    std::cout << (set.add(std::string("Some data")) ? "added" : "already exists") << std::endl;
    std::cout << (set.add(std::string("Some data")) ? "added" : "already exists") << std::endl;
    
    // check some data
    std::cout << (set.contains(std::string("Some data")) ? "yes" : "no") << std::endl;
    std::cout << (set.contains(std::string("some data")) ? "yes" : "no") << std::endl;

    // remove some data
    std::cout << (set.remove(std::string("Some data")) ? "contained" : "did not contain") << std::endl;
    std::cout << (set.remove(std::string("some data")) ? "contained" : "did not contain") << std::endl;

    set.add("More data");
    set.add("Even more data");

    for (char c = 'a'; c <= 'z'; ++c) {
        set.add(std::string(1, c));
    }

    for (const auto& data : set) {
        std::cout << data << std::endl;
    }
    
    return 0;
}

// Besides the standard requirements you can show us what you now. E.g. use
// namespaces, make the container iterable by adding iterators to your container,
// add a move constructor, make it super efficient, etc.

// Good luck!
