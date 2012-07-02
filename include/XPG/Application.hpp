#ifndef XpgApplicationHpp
#define XpgApplicationHpp

#include "Platform.hpp"

namespace XPG
{
    XpgClass Application
    {
        public:
            Application();
            ~Application();

            void Run();

        private:
            Application(const Application& application)
            {
                // no copying
            }

            Application& operator=(const Application& application)
            {
                // no copying
                return *this;
            }

            char _native[4];
    };
}

#endif
