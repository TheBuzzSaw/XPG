#include "../../include/XPG/Thread.hpp"
#include <pthread.h>
#include <cerrno>
#include <cstdlib>

namespace XPG
{
    struct ThreadMeta
    {
        pthread_t thread;
        Thread::Entry entry;
        volatile bool* running;
        void* data;
    };

    static void* CreateThread(void* data)
    {
        ThreadMeta* meta = (ThreadMeta*)data;
        *meta->running = true;
        meta->entry(meta->data);
        *meta->running = false;
        pthread_exit(0);
        return 0;
    }

    Thread::Thread() : _isRunning(false)
    {
        _native = malloc(sizeof(ThreadMeta));
        ThreadMeta* meta = (ThreadMeta*)_native;
        meta->running = &_isRunning;
    }

    Thread::~Thread()
    {
        free(_native);
    }

    void Thread::Start(Thread::Entry entry, void* data)
    {
        if (!_isRunning)
        {
            ThreadMeta* meta = (ThreadMeta*)_native;
            meta->entry = entry;
            meta->data = data;

            switch (pthread_create(&meta->thread, NULL, CreateThread, meta))
            {
                case 0:
                    break; // no error

                case EAGAIN: // The system lacked resources
                case EINVAL: // invalid attributes
                case EPERM: // Caller does not have appropriate permission
                default: // something else went wrong
                    break;
            }
        }
    }

    void Thread::Join()
    {
        if (_isRunning)
        {
            ThreadMeta* meta = (ThreadMeta*)_native;
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
