#include <iostream>
#include "OHNO.h"
using namespace std;
int main()
{
    cout << "Hello, World!" << endl;
    OHNO();
    auto [x,y] = make_pair(1,2);
    cout << "x: " << x << ", y: " << y << endl;
    return 0;
}
