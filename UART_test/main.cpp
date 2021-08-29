#include <iostream>
#include "uart.h"
using namespace std;

int main()
{
    char str[100] = {0};
    uart0.ReadLine(str);
    uart0.ReadLine(str);
    uart0.ReadLine(str);

    uart0.Transmit();
    uart0.ReadLine(str);

    cout << "Hello world!" << endl;
    return 0;
}
