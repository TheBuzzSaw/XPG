#include <XPG/Event.hpp>
#include <XPG/Stopwatch.hpp>
#include <XPG/NativeTimers.hpp>
#include <XPG/Thread.hpp>
#include <iostream>
using namespace std;

const char* const Letters = "ABCDEF";

void TestListener(void* data)
{
    XPG::Stopwatch stopwatch;
    stopwatch.Start();
    Sleep(XPG::TimeSpan::FromMilliseconds(150));
    cout << "timer slept for " << stopwatch.Elapsed() << endl;
    cout << "data: " << data << endl;
}

void Talk(void* data)
{
    const char* letter = (const char*)data;

    for (int i = 0; i < 4; ++i)
    {
        cout << "Greetings from thread " << *letter << endl;
        XPG::Sleep(XPG::TimeSpan::FromSeconds(1));
    }
}

int main(int argc, char** argv)
{
    XPG::ResetTimer();
    XPG::Event e;
    e.AddListener(TestListener);
    e.AddListener(TestListener, &e);
    e.AddListener(TestListener, &cout);
    e.Fire();

    XPG::Thread a;
    a.Start(Talk, (void*)(Letters + 0));

    XPG::Sleep(XPG::TimeSpan::FromMilliseconds(500));

    XPG::Thread b;
    b.Start(Talk, (void*)(Letters + 1));

    a.Join();
    b.Join();

    return 0;
}
