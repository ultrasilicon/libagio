#include "stream.h"

#include <functional>
#include <cstdlib>

using namespace Agio;


Stream::Stream()
{

}

Stream::~Stream()
{

}

int Stream::write(char *)
{
  return 0;
}

int Stream::write(char *, const size_t &)
{
  return 0;
}

int Stream::write(std::string)
{
  return 0;
}
