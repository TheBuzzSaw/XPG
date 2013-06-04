#ifndef XpgApplicationHpp
#define XpgApplicationHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    class XpgApi Application
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

            void* _native;
    };
}

#endif
