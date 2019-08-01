#ifndef AGIO_LIB_H
#define AGIO_LIB_H

#include "agio.h"

A_NS_BEGIN

class Lib
    : public AgioObject<uv_lib_t, Lib>
{
public:
    Lib();

    int open(const char* path);
    void close();
    int getSymbol(const char* name, void** ptr);
    const char* getError();
};

A_NS_END
#endif // AGIO_LIB_H
