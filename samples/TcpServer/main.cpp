#include <XPG/Network/Management.hpp>
#include <XPG/Network/Address32Query.hpp>
#include <XPG/Network/UdpSocket.hpp>
#include <XPG/Network/TcpSocket.hpp>
#include <XPG/Network/TcpListener.hpp>
#include <XPG/Joystick.hpp>
#include <XPG/JoystickManager.hpp>
#include <XPG/Clock.hpp>
#include <XPG/TimeSpan.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

void HttpGoogle()
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
            "GET / HTTP/1.1\r\n"
            "Host: www.google.com\r\n"
            "Connection: close\r\n"
            "\r\n";

        packet << request << XPG::Null;

        XPG::TcpSocket socket;

        if (socket.Open(address))
        {
            socket.Send(packet);

            ofstream fout("test.txt", ofstream::binary);

            while (fout && socket.Receive(packet))
            {
                const char* buffer =
                    reinterpret_cast<const char*>(packet.Buffer());

                fout.write(buffer, packet.ContentLength());
            }

            fout.close();
        }
    }
}

void SuperSimpleWebServer()
{
    XPG::TcpListener listener;
    XPG::UInt16 port = 8080;

    if (listener.Open(port))
    {
        cout << "listening on port " << port << endl;

        XPG::TcpSocket socket;

        if (socket.Open(listener))
        {
            cout << "connection established" << endl;

            XPG::Packet packet(2048);

            while (socket.Receive(packet))
            {
                cout.write((const char*)packet.Buffer(),
                    packet.ContentLength());

                socket.SetBlocking(false);
            }

            packet.Clear();

            const char* response =
                "HTTP/1.1 200 OK\r\n"
                "Connection: close\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n"
                "\r\n"
                "<html><head><title>XPG Server</title></head>"
                "<body><p>Hello, World! Text is da bomb.</p></body>"
                "</html>"
                ;

            packet << response;
            socket.Send(packet);
        }
        else
        {
            cout << "failed to accept connection" << endl;
        }
    }
    else
    {
        cout << "failed to listen on port " << port << endl;
    }
}

int main(int argc, char** argv)
{
    if (XPG::OpenSockets())
    {
        SuperSimpleWebServer();
        XPG::CloseSockets();
    }
    else
    {
        cerr << "failed to open sockets\n";
        return 1;
    }

    return 0;
}
