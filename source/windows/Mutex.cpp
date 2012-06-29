#include "../../include/XPG/Mutex.hpp"
#include "Windows.hpp"
#include <cstdlib>

namespace XPG
{
    Mutex::Mutex()
    {
        _native = malloc(sizeof(HANDLE));
        HANDLE* handle = (HANDLE*)_native;
        *handle = CreateMutex(NULL, FALSE, NULL);
    }

    Mutex::~Mutex()
    {
        HANDLE* handle = (HANDLE*)_native;
        CloseHandle(*handle);
        free(_native);
    }

    void Mutex::Lock()
    {
        HANDLE* handle = (HANDLE*)_native;
        WaitForSingleObject(*handle, INFINITE);
    }

    void Mutex::Unlock()
    {
        HANDLE* handle = (HANDLE*)_native;
        if (!ReleaseMutex(*handle))
        {
            // TODO: error checking
        }
    }
}
