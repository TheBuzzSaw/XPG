#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
using namespace std;

int main(int argc, char** argv)
{
    wchar_t text[] = { 0xa2, 0x00 };
    wprintf(L"TESTING: %ls\n", text);
    return 0;
}
