#include "lib.h"

using namespace Agio;

Lib::Lib()
    : AgioObject()
{}

int Lib::open(const char* path)
{
    return uv_dlopen(path, obj_);
}

void Lib::close()
{
    uv_dlclose(obj_);
}

int Lib::getSymbol(const char* name, void** ptr)
{
    return uv_dlsym(obj_, name, ptr);
}

const char* Lib::getError()
{
    return uv_dlerror(obj_);
}
