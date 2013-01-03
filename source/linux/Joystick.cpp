#include "../../include/XPG/Joystick.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

namespace XPG
{

    struct js_event
    {
        unsigned int time;
        short value;
        unsigned char type;
        unsigned char number;
    };

    Joystick::Joystick(UInt32 numJoystick)
    {
        _numJoystick = numJoystick;

        _numAxes = 0;
        _numButtons = 0;
        _numHats = 0;
        _axisMinimums = 0;
        _axisMaximums = 0;

        stringstream fileName;
        fileName << "/dev/input/js" << numJoystick;

        struct stat buffer;

        if (stat(fileName.str().c_str(), &buffer) > -1)
        {
            int fd = open(fileName.str().c_str(), O_RDONLY | O_NONBLOCK);

            if (fd > -1)
            {
                char numberOfDevices = 0;
                if (ioctl(fd, JSIOCGAXES, &numberOfDevices) > -1)
                {
                    _numAxes = numberOfDevices;
                    _axisMinimums = new Int32[_numAxes];
                    _axisMaximums = new Int32[_numAxes];

                    for (unsigned int i = 0; i < _numAxes; ++i)
                    {
                        _axisMinimums[i] = -32768;
                        _axisMaximums[i] = 32767;
                    }
                }

                if (ioctl(fd, JSIOCGBUTTONS, &numberOfDevices) > -1)
                {
                    _numButtons = numberOfDevices;
                }

                char name[1024];
                memset(name, 0, 1024);
                if (ioctl(fd, JSIOCGNAME(1024), name) < 0)
                    strncpy(name, "Unknown", 1024);

                cerr << "Joystick " << numJoystick << " details:" << endl;
                cerr << "Num Axes: " << _numAxes << endl;
                cerr << "Num Buttons: " << _numButtons << endl;
                cerr << "Name: " << name << endl;

                close(fd);
            }
        }

        _axisStates = new Int32[_numAxes];
        _buttonStates = new bool[_numButtons];
        _hatStates = new Int32[_numHats];
    }


    void Joystick::PollState()
    {
        struct stat buffer;

        string baseFilePath = "/dev/input/js";
        stringstream filePath;
        filePath << baseFilePath << _numJoystick;
        cerr << "opening " << filePath.str() << endl;

        if (stat(filePath.str().c_str(), &buffer) > -1)
        {
            int fd = open(filePath.str().c_str(), O_RDONLY | O_NONBLOCK);

            if (fd > -1)
            {
                js_event e;

                while (read(fd, &e, sizeof(js_event)) > -1)
                {
                    switch (e.type)
                    {
                        case JS_EVENT_BUTTON | JS_EVENT_INIT:
                        {
                            cerr << "button " << (int)e.number << " set to " << e.value << endl;
                            break;
                        }

                        case JS_EVENT_BUTTON & ~JS_EVENT_INIT:
                        {
                            cerr << "button " << (int)e.number << " changed to " << e.value << endl;
                            break;
                        }

                        case JS_EVENT_AXIS | JS_EVENT_INIT:
                        {
                            cerr << "axis " << (int)e.number << " set to " << e.value << endl;
                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }
                }

                close(fd);
            }
        }
    }

    UInt32 Joystick::NumJoysticks()
    {
        string baseFileName = "/dev/input/js";
        stringstream filePath;

        int i = 0;

        filePath << baseFileName << i;

        struct stat buffer;

        while (stat(filePath.str().c_str(), &buffer) == 0)
        {
            ++i;
            filePath.str(string());
            filePath.clear();
            filePath << baseFileName << i;
        }

        return i;
    }

}
