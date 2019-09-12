#include <iostream>
#include <libagio/lib.h>

using namespace std;
using namespace Agio;

int main()
{
  Lib* lib = new Lib();
  if(lib->open("./hello.dylib") == -1)
    {
      cout << lib->getError() << endl;
      return 1;
    }

  int (*fun) (void);
  if(lib->getSymbol("main", (void**) &fun) == -1)
    {
      cout << lib->getError() << endl;
      return 1;
    }
  fun();

  return 0;
}
