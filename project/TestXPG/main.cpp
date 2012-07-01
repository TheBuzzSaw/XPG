#include <XPG/Event.hpp>
#include <XPG/Stopwatch.hpp>
#include <XPG/Clock.hpp>
#include <XPG/Thread.hpp>
#include <XPG/Mutex.hpp>
#include <XPG/Window.hpp>
#include <iostream>
using namespace std;

const char* const Letters = "ABCDEF";
XPG::Mutex mutex;

void TestListener(void* data)
{
    XPG::Stopwatch stopwatch;
    stopwatch.Start();
    XPG::Sleep(XPG::TimeSpan::FromMilliseconds(150));
    cout << "timer slept for " << stopwatch.Elapsed() << endl;
    cout << "data: " << data << endl;
}

void Talk(void* data)
{
    const char* letter = (const char*)data;

    for (int i = 0; i < 4; ++i)
    {
        cout << "Greetings from thread " << *letter << endl;
        XPG::Sleep(XPG::TimeSpan::FromMilliseconds(500));
        mutex.Lock();
        XPG::Sleep(XPG::TimeSpan::FromMilliseconds(500));
        mutex.Unlock();
    }
}

void MakeWindow(void* data)
{
    (void)data;
    XPG::Window window;
    window.Run();
}

int main(int argc, char** argv)
{
    XPG::ResetTimer();

    cout << XPG::DateTime(1601, 1, 1).Ticks() << endl;
    cout << XPG::DateTime::UtcTime() << endl;
    cout << XPG::HighResolutionUtcTime() << endl;
    cout << XPG::DateTime::LocalTime() << endl;
    cout << XPG::HighResolutionLocalTime() << endl;

    XPG::Event e;
    e.AddListener(TestListener);
    e.AddListener(TestListener, &e);
    e.AddListener(TestListener, &cout);
    e.Fire();

    MakeWindow(NULL);

    XPG::Thread a;
    a.Start(Talk, (void*)(Letters + 0));

    XPG::Thread b;
    b.Start(Talk, (void*)(Letters + 1));

    a.Join();
    cout << "joined thread A" << endl;
    b.Join();
    cout << "joined thread B" << endl;

    return 0;
}
