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
        volatile bool running;
        Thread::Entry entry;
        void* data;
    };

    DWORD WINAPI ThreadMain(LPVOID data)
    {
        ThreadMeta* meta = (ThreadMeta*)data;
        meta->entry(meta->data);
        meta->running = false;
        return 0;
    }

    Thread::Thread()
    {
        memset(_native, 0, sizeof(_native));
    }

    Thread::~Thread()
    {
        ThreadMeta* meta = (ThreadMeta*)_native;

        if (meta->handle != NULL)
            CloseHandle(meta->handle);
    }

    bool Thread::IsRunning() const
    {
        const ThreadMeta* meta = (const ThreadMeta*)_native;
        return meta->running;
    }

    void Thread::Start(Thread::Entry entry, void* data)
    {
        ThreadMeta* meta = (ThreadMeta*)_native;

        if (!meta->running)
        {
            meta->entry = entry;
            meta->data = data;
            meta->running = true;
            meta->handle = CreateThread(NULL, 0, ThreadMain, meta, 0,
                &meta->id);

            if (meta->handle == NULL)
                meta->running = false;
        }
    }

    void Thread::Join()
    {
        ThreadMeta* meta = (ThreadMeta*)_native;

        if (meta->running)
        {
            WaitForSingleObject(meta->handle, INFINITE);
        }
    }
}
