#ifndef XpgNetworkHpp
#define XpgNetworkHpp

#include "../include/XPG/Platform.hpp"

#ifdef XpgPlatformWindows
#   include "windows/Windows.hpp"
#   include <winsock2.h>
#   include <ws2tcpip.h>
typedef int socklen_t;
#else
#   include <cstdio>
#   include <cstdlib>
#   include <cerrno>
#   include <cstring>

#   include <unistd.h>
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#   include <netdb.h>

#   define SOCKET int
#   define INVALID_SOCKET -1
#   define closesocket close
#endif

#endif
