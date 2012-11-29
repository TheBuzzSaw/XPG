#include <XPG/Network/Management.hpp>
#include <XPG/Network/Address32Query.hpp>
#include <XPG/Network/UdpSocket.hpp>
#include <XPG/Network/TcpSocket.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
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

            if (query.Count() > 0)
            {
                XPG::Address32 address = query.GetResult(0);

                XPG::Packet packet(2048);
                const char* request =
                    "GET / HTTP/1.1\n"
                    "Host: www.google.com\n"
                    "Connection: close\n"
                    "\n\n";
                //packet << request << XPG::Int8(0);
                packet.Write(request, strlen(request) + 1);

                XPG::TcpSocket socket;

                if (socket.Open(address))
                {
                    socket.SendAll(packet);

                    ofstream fout("test.txt", ofstream::binary);

                    while (fout && socket.Receive(packet))
                    {
                        const char* buffer = (const char*)packet.Buffer();
                        for (XPG::UInt16 i = 0; i < packet.ContentLength(); ++i)
                            fout.write(buffer + i, 1);
                    }

                    fout.close();
                }
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
