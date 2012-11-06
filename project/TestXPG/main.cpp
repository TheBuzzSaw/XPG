#include <XPG/Event.hpp>
#include <XPG/Stopwatch.hpp>
#include <XPG/Clock.hpp>
#include <XPG/Thread.hpp>
#include <XPG/Mutex.hpp>
#include <XPG/Window.hpp>
#include <XPG/Application.hpp>
#include <XPG/Array.hpp>
#include <iostream>
#include <string>
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
}

void SendAsyncDrawCalls(void* data)
{
    XPG::Window* window = (XPG::Window*)data;

    for (int i = 0; i < 8; ++i)
    {
        XPG::Sleep(XPG::TimeSpan::FromMilliseconds(500));
        window->Draw();
    }
}

void OnLeftMouseButtonDown(XPG::MouseState& currentState)
{
    cerr << "Hello!!!! x,y: " << currentState.X() << ", " << currentState.Y() << endl;
}

int main(int argc, char** argv)
{
    XPG::ResetTimer();

//    XPG::Array<string> array;
//    array.Add("Herro");
//    array.Add("Avast ye!");
//    array.Add("Shweet");
//
//    XPG::Array<string> anotherArray = array;
//
//    cout << "array has capacity " << anotherArray.Capacity() << endl;
//
//    for (size_t i = 0; i < anotherArray.Size(); ++i)
//    {
//        cout << anotherArray[i] << endl;
//    }

    cout << XPG::DateTime(1601, 1, 1).Ticks() << endl;
    cout << XPG::DateTime(9999, 12, 31).Ticks() << endl;
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

    XPG::Thread c;

    //MakeWindow();
    XPG::Application application;
    XPG::Window window[4];
    window[0].SetTitle("XPG Main Window");
    window[1].SetTitle("XPG Mini Map");

    window[0].SetOnLeftMouseButtonDown(OnLeftMouseButtonDown);
    //c.Start(SendAsyncDrawCalls, window);
    application.Run();

    a.Join();
    cout << "joined thread A" << endl;
    b.Join();
    cout << "joined thread B" << endl;

    return 0;
}
