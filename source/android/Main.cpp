#include "../../include/XPG/Main.hpp"
#include "AndroidApp.hpp"

namespace XPG
{
    static android_app* theAndroidApp = 0;
    
    android_app* GetAndroidApp()
    {
        return theAndroidApp;
    }
    
    void SetAndroidApp(android_app* androidApp)
    {
        theAndroidApp = androidApp;
    }
}

void android_main(android_app* androidApp)
{
    XPG::SetAndroidApp(androidApp);
    XpgMain();
}

