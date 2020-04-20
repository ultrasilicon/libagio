#ifndef AGIO_TEST_UTILS_H
#define AGIO_TEST_UTILS_H

#include <string>
#include <vector>
#include <climits>
#include <cstdlib>
#include <algorithm>

using namespace std;

namespace TestUtils {

namespace Random {

static string randomStr(const size_t& length)
{
    string str(length,0);
    generate_n(str.begin(), length,  []() {
        const char charset[] = " 0123456789abcdefghijklmnopqrstuvwxyz\0\n\t";
        return charset[ static_cast<unsigned long>(rand()) % (sizeof(charset) - 1) ];
    });
    return str;
}

static string randomName(const size_t& length)
{
    string str(length,0);
    generate_n(str.begin(), length,  []() {
        const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyz";
        return charset[ static_cast<unsigned long>(rand()) % (sizeof(charset) - 1) ];
    });
    return str;
}


}

}

#endif // AGIO_TEST_UTILS_H
