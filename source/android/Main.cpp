#include "../../include/XPG/Main.hpp"
#include "AndroidApp.hpp"

namespace XPG
{
    static android_app* theAndroidApp = 0;
    
    android_app* GetAndroidApp()
    {
        return theAndroidApp;
    }
}

void android_main(android_app* androidApp)
{
    XPG::theAndroidApp = androidApp;
    app_dummy();
    XpgMain();
}

