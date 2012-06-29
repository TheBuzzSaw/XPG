#include <XPG/Event.hpp>
#include <XPG/Stopwatch.hpp>
#include <XPG/NativeTimers.hpp>
#include <iostream>
using namespace std;

void TestListener(void* inData)
{
    XPG::Stopwatch stopwatch;
    stopwatch.Start();
    Sleep(XPG::TimeSpan::FromMilliseconds(1500));
    cout << "timer slept for " << stopwatch.Elapsed() << endl;
    cout << "data: " << inData << endl;
}

int main(int argc, char** argv)
{
    XPG::ResetTimer();
    XPG::Event e;
    e.AddListener(TestListener);
    e.AddListener(TestListener, &e);
    e.AddListener(TestListener, &cout);

    e.Fire();

    return 0;
}
