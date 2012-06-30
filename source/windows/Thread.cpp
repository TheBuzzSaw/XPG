#include "../../include/XPG/Thread.hpp"
#include "Windows.hpp"
#include <cstdlib>
#include <cstring>

namespace XPG
{
    struct ThreadMeta
    {
        DWORD id;
        HANDLE handle;
        volatile bool* running;
        Thread::Entry entry;
        void* data;
    };

    DWORD WINAPI ThreadMain(LPVOID data)
    {
        ThreadMeta* meta = (ThreadMeta*)data;
        meta->entry(meta->data);
        *meta->running = false;
        return 0;
    }

    Thread::Thread() : _isRunning(false)
    {
        _native = malloc(sizeof(ThreadMeta));
        memset(_native, 0, sizeof(ThreadMeta));
        ThreadMeta* meta = (ThreadMeta*)_native;
        meta->running = &_isRunning;
    }

    Thread::~Thread()
    {
        ThreadMeta* meta = (ThreadMeta*)_native;
        CloseHandle(meta->handle);
        free(_native);
    }

    void Thread::Start(Thread::Entry entry, void* data)
    {
        if (!_isRunning)
        {
            ThreadMeta* meta = (ThreadMeta*)_native;
            meta->entry = entry;
            meta->data = data;
            meta->handle = CreateThread(NULL, 0, ThreadMain, meta, 0,
                &meta->id);

            _isRunning = meta->handle != NULL;
        }
    }

    void Thread::Join()
    {
        if (_isRunning)
        {
            ThreadMeta* meta = (ThreadMeta*)_native;
            WaitForSingleObject(meta->handle, INFINITE);
        }
    }
}
