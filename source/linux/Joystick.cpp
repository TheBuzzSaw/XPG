#include "../../include/XPG/Joystick.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <stdio.h>
using namespace std;

namespace XPG
{

    struct JoystickMeta
    {
        int fileDescriptor;
    };

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

        memset(_native, 0, 1024);

        stringstream fileName;
        fileName << "/dev/input/js" << numJoystick;

        struct stat buffer;

        if (stat(fileName.str().c_str(), &buffer) > -1)
        {
            JoystickMeta* meta = reinterpret_cast<JoystickMeta*>(_native);

            meta->fileDescriptor = open(fileName.str().c_str(), O_RDONLY | O_NONBLOCK);

            if (meta->fileDescriptor > -1)
            {
                cerr << "file descriptor: " << meta->fileDescriptor << endl;
                char numberOfDevices = 0;
                if (ioctl(meta->fileDescriptor, JSIOCGAXES, &numberOfDevices) > -1)
                {
                    _numAxes = numberOfDevices;

                    _axisMinimums = new Int32[_numAxes];
                    _axisMaximums = new Int32[_numAxes];

                    for (UInt32 i = 0; i < _numAxes; ++i)
                    {
                        _axisMinimums[i] = -32767;
                        _axisMaximums[i] = 32768;
                    }
                }

                if (ioctl(meta->fileDescriptor, JSIOCGBUTTONS, &numberOfDevices) > -1)
                {
                    _numButtons = numberOfDevices;
                }

                char name[1024];
                memset(name, 0, 1024);
                if (ioctl(meta->fileDescriptor, JSIOCGNAME(1024), name) < 0)
                    strncpy(name, "Unknown", 1024);

                cerr << "Joystick " << numJoystick << " details:" << endl;
                cerr << "Num Axes: " << _numAxes << endl;
                cerr << "Num Buttons: " << _numButtons << endl;
                cerr << "Name: " << name << endl;

                close(meta->fileDescriptor);
            }
        }

        _axisStates = new Int32[_numAxes];
        _buttonStates = new bool[_numButtons];
        _hatStates = new Int32[_numHats];
    }


    Joystick::Joystick(const Joystick& joystick)
    {
        _numJoystick = joystick._numJoystick;
        _numAxes = joystick._numAxes;
        _numButtons = joystick._numButtons;
        _numHats = joystick._numHats;

        _axisStates = new Int32[_numAxes];
        _buttonStates = new bool[_numButtons];
        _hatStates = new Int32[_numHats];

        _axisMinimums = 0;
        _axisMaximums = 0;

        if (joystick._axisMinimums != 0)
        {
            _axisMinimums = new Int32[_numAxes];

            for (UInt32 i = 0; i < _numAxes; ++i)
            {
                _axisMinimums[i] = joystick._axisMinimums[i];
            }
        }

        if (joystick._axisMaximums != 0)
        {
            _axisMaximums = new Int32[_numAxes];

            for (UInt32 i = 0; i < _numAxes; ++i)
            {
                _axisMaximums[i] = joystick._axisMaximums[i];
            }
        }

        for (UInt32 i = 0; i < _numAxes; ++i)
        {
            _axisStates[i] = joystick._axisStates[i];
        }

        for (UInt32 i = 0; i < _numButtons; ++i)
        {
            _buttonStates[i] = joystick._buttonStates[i];
        }

        for (UInt32 i = 0; i < _numHats; ++i)
        {
            _hatStates[i] = joystick._hatStates[i];
        }

        JoystickMeta* meta = reinterpret_cast<JoystickMeta*>(_native);

        meta->fileDescriptor = ((JoystickMeta*)joystick._native)->fileDescriptor;

    }



    Joystick::~Joystick()
    {

        if (_axisMinimums != 0)
            delete [] _axisMinimums;

        if (_axisMaximums != 0)
            delete [] _axisMaximums;

        if (_axisStates != 0)
            delete [] _axisStates;

        if (_buttonStates != 0)
            delete [] _buttonStates;

        if (_hatStates != 0)
            delete [] _hatStates;


        JoystickMeta* meta = reinterpret_cast<JoystickMeta*>(_native);
        close(meta->fileDescriptor);
    }

    Joystick& Joystick::operator =(const Joystick& joystick)
    {
        _numJoystick = joystick._numJoystick;
        _numAxes = joystick._numAxes;
        _numButtons = joystick._numButtons;
        _numHats = joystick._numHats;

        if (_axisMinimums != 0)
            delete [] _axisMinimums;

        if (_axisMaximums != 0)
            delete [] _axisMaximums;

        if (joystick._axisMinimums != 0)
        {
            _axisMinimums = new Int32[_numAxes];

            for (UInt32 i = 0; i < _numAxes; ++i)
            {
                _axisMinimums[i] = joystick._axisMinimums[i];
            }
        }

        if (joystick._axisMaximums != 0)
        {
            _axisMaximums = new Int32[_numAxes];

            for (UInt32 i = 0; i < _numAxes; ++i)
            {
                _axisMaximums[i] = joystick._axisMaximums[i];
            }
        }

        if (_axisStates != 0)
            delete [] _axisStates;

        if (_buttonStates != 0)
            delete [] _buttonStates;

        if (_hatStates != 0)
            delete [] _hatStates;

        _axisStates = new Int32[_numAxes];
        _buttonStates = new bool[_numButtons];
        _hatStates = new Int32[_numHats];

        for (UInt32 i = 0; i < _numAxes; ++i)
        {
            _axisStates[i] = joystick._axisStates[i];
        }

        for (UInt32 i = 0; i < _numButtons; ++i)
        {
            _buttonStates[i] = joystick._buttonStates[i];
        }

        for (UInt32 i = 0; i < _numHats; ++i)
        {
            _hatStates[i] = joystick._hatStates[i];
        }

        JoystickMeta* meta = reinterpret_cast<JoystickMeta*>(_native);

        meta->fileDescriptor = ((JoystickMeta*)joystick._native)->fileDescriptor;

        return *this;
    }


    void Joystick::PollState()
    {
        struct stat buffer;

        string baseFilePath = "/dev/input/js";
        stringstream filePath;
        filePath << baseFilePath << _numJoystick;
        //cerr << "opening " << filePath.str() << endl;

//        JoystickMeta* meta = reinterpret_cast<JoystickMeta*>(_native);
//
//        if (meta->fileDescriptor > -1)
//        {
//            //cerr << "file descriptor: " << meta->fileDescriptor << endl;
//            js_event e;
//
//            while (read(meta->fileDescriptor, &e, sizeof(js_event)) > 0)
//            {
//                switch (e.type)
//                {
//                    case JS_EVENT_BUTTON | JS_EVENT_INIT:
//                    case JS_EVENT_BUTTON:
//                    {
//                        cerr << "button " << (int)e.number << " set to " << e.value << endl;
//                        _buttonStates[e.number] = e.value == 1;
//                        break;
//                    }
//
////                    case JS_EVENT_BUTTON & ~JS_EVENT_INIT:
////                    {
////                        cerr << "button " << (int)e.number << " changed to " << e.value << endl;
////                        break;
////                    }
//
//                    case JS_EVENT_AXIS | JS_EVENT_INIT:
//                    case JS_EVENT_AXIS:
//                    {
//                        cerr << "axis " << (int)e.number << " set to " << e.value << endl;
//                        _axisStates[e.number] = e.value;
//                        break;
//                    }
//
//                    default:
//                    {
//                        break;
//                    }
//                }
//            }
//
//            if (errno != EAGAIN)
//            {
//                //perror("something went wrong");
//                errno = 0;
//            }
//        }

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
                        case JS_EVENT_BUTTON:
                        {
                            //cerr << "button " << (int)e.number << " set to " << e.value << endl;
                            _buttonStates[e.number] = e.value == 1;
                            break;
                        }

//                        case JS_EVENT_BUTTON & ~JS_EVENT_INIT:
//                        {
//                            cerr << "button " << (int)e.number << " changed to " << e.value << endl;
//                            break;
//                        }

                        case JS_EVENT_AXIS | JS_EVENT_INIT:
                        case JS_EVENT_AXIS:
                        {
                            //cerr << "axis " << (int)e.number << " set to " << e.value << endl;
                            _axisStates[e.number] = e.value;
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
