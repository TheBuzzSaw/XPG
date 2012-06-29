#include "../../include/XPG/Mutex.hpp"
#include <pthread.h>
#include <cstdlib>

namespace XPG
{
    Mutex::Mutex()
    {
        _native = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_t* mutex = (pthread_mutex_t*)_native;
        pthread_mutex_init(mutex, NULL);
    }

    Mutex::~Mutex()
    {
        pthread_mutex_t* mutex = (pthread_mutex_t*)_native;
        pthread_mutex_destroy(mutex);
        free(_native);
    }

    void Mutex::Lock()
    {
        pthread_mutex_t* mutex = (pthread_mutex_t*)_native;
        pthread_mutex_lock(mutex);
    }

    void Mutex::Unlock()
    {
        pthread_mutex_t* mutex = (pthread_mutex_t*)_native;
        pthread_mutex_unlock(mutex);
    }
}
