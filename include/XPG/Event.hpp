#ifndef XpgEventHpp
#define XpgEventHpp

namespace XPG
{
    class Event
    {
        public:
            typedef void (*Listener)(void*);

            Event();
            virtual ~Event();

            void AddListener(Listener listener, void* userData = 0);
            void RemoveListener(Listener listener, void* userData = 0);
            void Fire();

        private:
            struct Handler
            {
                Listener listener;
                void* userData;
                Handler* nextHandler;
            };

            Handler* _firstHandler;
    };
}

#endif
