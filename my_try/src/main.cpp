#include <iostream>
#include "OHNO.h"
using namespace std;
int main()
{
    cout << "Hello, World!" << endl;
    cout << OHNO("Hello, OHNO!") << endl;
    cout << "Length of string: " << strlen("Hello, World!") << endl;
    auto [x,y] = make_pair(1,2);
    cout << "x: " << x << ", y: " << y << endl;
    return 0;
}
