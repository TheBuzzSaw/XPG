#include <XPG/Network/Management.hpp>
#include <XPG/Network/Address32Query.hpp>
#include <XPG/Network/UdpSocket.hpp>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
    if (XPG::OpenSockets())
    {
        {
            XPG::Address32Query query("google.com", "80");

            cout << query.Count() << " results" << endl;

            for (XPG::UInt32 i = 0; i < query.Count(); ++i)
            {
                XPG::Address32 a = query.GetResult(i);
                cout << a.A() << "." << a.B() << "." << a.C() << "." << a.D()
                    << ":" << a.Port() << endl;
            }
        }

        XPG::CloseSockets();
    }
    else
    {
        cerr << "failed to open sockets\n";
        return 1;
    }

    return 0;
}
