#include<iostream>
#include"defns.h"
using namespace std;

void preprocess(const char*,const char*);

int main(int argc, char** argv)
{
    symbol symbols[NSYMBOLS];

    for (int i = 0; i < NSYMBOLS; i++)
    {
        symbols[i].symbol = 0;
        symbols[i].freq = 0;

    }
    char c;
    while (cin.get(c))
    {
        symbols[c].symbol = c;
        symbols[c].freq += 1;
    }

    for (int i = 0; i < NSYMBOLS; ++i)
    {
        if(symbols[i].freq != 0)
        {
            cout << i << "\t" << symbols[i].freq << "\n";
        }
    }
    return 0;
}

