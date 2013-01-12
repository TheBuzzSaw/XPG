#ifndef XpgApplicationMetaHpp
#define XpgApplicationMetaHpp

#include "../../include/XPG/DataTypes.hpp"
#include "WindowMeta.hpp"

namespace XPG
{
    struct ApplicationMeta
    {
        Int32 referenceCount;
        Int32 windowCount;
        WindowMeta* windowMeta;
    };

    extern ApplicationMeta theApplicationMeta;
}

#endif
