#include <XPG/Network/Management.hpp>
#include <XPG/Network/UdpSocket.hpp>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
    if (XPG::OpenSockets())
    {
        if (argc > 1)
        {
            XPG::UdpSocket socket;
            socket.Open(28555);
            const char* message = "HERRO";
            if (socket.Send(XPG::Address32(31313), message, 5))
                cout << "sent message";
            else
                cout << "failed to send message";
        }
        else
        {
            XPG::UdpSocket socket;
            socket.Open(31313);
            char buffer[256] = "";

            XPG::Address32 source;
            cout << "received " << socket.Receive(source, buffer, 256)
                << " bytes: " << buffer << endl;
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
