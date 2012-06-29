#ifndef XpgMutexHpp
#define XpgMutexHpp

namespace XPG
{
    class Mutex
    {
        public:
            Mutex();
            ~Mutex();

            void Lock();
            void Unlock();

        private:
            void* _native;
    };
}

#endif
