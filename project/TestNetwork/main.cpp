#include <XPG/Network/Management.hpp>
#include <XPG/Network/UdpSocket.hpp>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
    if (XPG::OpenSockets())
    {
        XPG::Packet32 packet(256);

        if (argc > 1)
        {
            packet.Write("HERRO", 5);
            packet.Address(XPG::Address32(31313));

            XPG::UdpSocket socket;
            socket.Open(28555);
            if (socket.Send(packet))
                cout << "sent message";
            else
                cout << "failed to send message";
        }
        else
        {
            XPG::UdpSocket socket;
            socket.Open(31313);

            if (socket.Receive(packet))
            {
                cout << "received " << packet.Position()
                    << " bytes: " << packet.Buffer() << endl;
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
