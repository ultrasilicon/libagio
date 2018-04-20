#include <iostream>
#include <../../src/PFile.h>

using namespace std;

int main()
{
  Parsley::Loop *l = Parsley::Loop::defaultLoop();
  Parsley::File file2(l);
  file2.open("/Volumes/VirtualDisk/a.txt", O_RDWR | O_CREAT, 0755, Parsley::SyncMode);
  std::string str = "0123456789abcdefghijklmnopqrstuvwxyz";
  std::cout<<file2.write(str, Parsley::SyncMode);
  file2.close();

  return 0;
}
