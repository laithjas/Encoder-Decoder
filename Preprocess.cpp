#include<iostream>
#include"defns.h"
using namespace std;

int main(int argc, char** argv)
{
  symbol symbols[NSYMBOLS];
  // initializing symbols array
  for (int i = 0; i < NSYMBOLS; i++)
  {
    symbols[i].symbol = 0;
    symbols[i].freq = 0;
  }
  char c;
  // reading from stdin, and incrementing freq of chars
  while (cin.get(c))
  {
    symbols[c].symbol = c;
    symbols[c].freq += 1;
  }
  symbols[10].freq +=1;

  // wrting to cout (writing to the pre-file.txt)
  for (int i = 0; i < NSYMBOLS; ++i)
  {
    if(symbols[i].freq != 0)
    {
      cout << i << "\t" << symbols[i].freq << "\n";
    }
  }
  return 0;
}
