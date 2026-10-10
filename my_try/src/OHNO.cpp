#include "OHNO.h"
#include <iostream>
using namespace std;

std::string OHNO(const std::string& s)
{
    return "OHNO: " + s;
}

int strlen(const std::string& s)
{
    return static_cast<int>(s.length());
}