// #include "parser.h"

#include <vector>
#include <string>

#include <fstream>
#include <iostream>

#include <unistd.h>

using namespace std;

// using std::vector;
// using std::string;

#ifndef _UTILS_H_
#define _UTILS_H_

template<typename T>
vector<T> split(const T & str, const T & delimiters) {
    vector<T> v;
    typename T::size_type start = 0;
    auto pos = str.find_first_of(delimiters, start);
    while(pos != T::npos)
    {
        if(pos != start) // ignore empty tokens
            v.emplace_back(str, start, pos - start);
        start = pos + 1;
        pos = str.find_first_of(delimiters, start);
    }
    if(start < str.length()) // ignore trailing delimiter
        v.emplace_back(str, start, str.length() - start); // add what's left of the string
    return v;
}



inline bool checkFileExistance(string filename)
{
    return (access(filename.c_str(), F_OK) != -1);
}

#endif
