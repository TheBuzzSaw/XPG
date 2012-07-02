#include <XPG/Event.hpp>
#include <XPG/Stopwatch.hpp>
#include <XPG/Clock.hpp>
#include <XPG/Thread.hpp>
#include <XPG/Mutex.hpp>
#include <XPG/Window.hpp>
#include <XPG/Array.hpp>
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

void MakeWindow()
{
    XPG::Window windows[3];
    XPG::Window::Run();
}

int main(int argc, char** argv)
{
    XPG::ResetTimer();

    XPG::Array<int> array;
    array.Add(13);
    array.Add(39);
    array.Add(-1);

    cout << "array has capacity " << array.Capacity() << endl;

    for (size_t i = 0; i < array.Size(); ++i)
    {
        cout << array[i] << endl;
    }

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

    XPG::Thread a;
    XPG::Thread b;

    a.Start(Talk, (void*)(Letters + 0));
    b.Start(Talk, (void*)(Letters + 1));

    MakeWindow();

    a.Join();
    cout << "joined thread A" << endl;
    b.Join();
    cout << "joined thread B" << endl;

    return 0;
}
