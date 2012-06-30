#include "../../include/XPG/Thread.hpp"
#include <pthread.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include <iostream>

namespace XPG
{
    struct ThreadMeta
    {
        pthread_t thread;
        Thread::Entry entry;
        volatile bool running;
        void* data;
    };

    static void* CreateThread(void* data)
    {
        ThreadMeta* meta = (ThreadMeta*)data;
        meta->entry(meta->data);
        meta->running = false;
        pthread_exit(0);
        return 0;
    }

    Thread::Thread()
    {
        memset(_native, 0, sizeof(_native));
        ThreadMeta* meta = (ThreadMeta*)_native;
        meta->running = false;
    }

    Thread::~Thread()
    {
    }

    bool Thread::IsRunning() const
    {
        ThreadMeta* meta = (ThreadMeta*)_native;
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

            switch (pthread_create(&meta->thread, NULL, CreateThread, meta))
            {
                case 0:
                    break; // no error

                case EAGAIN: // The system lacked resources
                case EINVAL: // invalid attributes
                case EPERM: // Caller does not have appropriate permission
                default: // something else went wrong
                    meta->running = false;
                    break;
            }
        }
    }

    void Thread::Join()
    {
        ThreadMeta* meta = (ThreadMeta*)_native;

        if (meta->running)
        {
            switch (pthread_join(meta->thread, NULL))
            {
                case 0: break; // no error

                case EINVAL: break; // Does not refer to a joinable thread
                case ESRCH: break; // No thread found corresponding to handle
                case EDEADLK: break; // A deadlock was detected
                default: {} // something else went wrong
            }
        }
    }
}
