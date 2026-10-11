#include <iostream>
#include "tyloo.h"

int main()
{
    int a = 5;
    int b = 10;
    int sum = add(a,b);
    std::cout << "Sum of " << a << " and " << b << " is: " << sum << std::endl;
    return 0;
}