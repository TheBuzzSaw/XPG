#ifndef XpgEventHpp
#define XpgEventHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    struct ResizeDetails
    {
        int width;
        int height;
    };

    XpgClass Event
    {
        public:
            struct Details
            {
                void* source;
                void* userData;
                UInt8 data[64];
            };

            typedef void (*Callback)(Details&);

            Event();
            Event(const Event& other);
            ~Event();

            Event& operator=(const Event& other);

            void Aim(Callback callback, void* userData = 0);
            void Clear();
            void Fire(Details& details) const;

        private:
            Callback _callback;
            void* _userData;
    };
}

#endif
