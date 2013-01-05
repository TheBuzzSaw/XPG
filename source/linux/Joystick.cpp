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
        Int32 hatState[2];
    };

    struct js_event
    {
        unsigned int time;
        short value;
        unsigned char type;
        unsigned char number;
    };

    int OpenJoystickFileDescriptor(UInt32 numJoystick)
    {
        int result = -1;

        stringstream fileName;
        fileName << "/dev/input/js" << numJoystick;

        struct stat buffer;

        if (stat(fileName.str().c_str(), &buffer) > -1)
        {
            result = open(fileName.str().c_str(), O_RDONLY | O_NONBLOCK);
        }

        return result;
    }

    Joystick::Joystick(UInt32 numJoystick)
    {
        _numJoystick = numJoystick;

        _numAxes = 0;
        _numButtons = 0;
        _numHats = 0;
        _axisMinimums = 0;
        _axisMaximums = 0;

        memset(_native, 0, 1024);

        JoystickMeta* meta = reinterpret_cast<JoystickMeta*>(_native);

        meta->hatState[0] = 0;
        meta->hatState[1] = 0;

        meta->fileDescriptor = OpenJoystickFileDescriptor(numJoystick);

        if (meta->fileDescriptor > -1)
        {
            cerr << "file descriptor: " << meta->fileDescriptor << endl;
            char numberOfDevices = 0;
            if (ioctl(meta->fileDescriptor, JSIOCGAXES, &numberOfDevices) > -1)
            {
                _numAxes = numberOfDevices;

                //In linux, the last 2 axes appear to always be the hat
                if (numberOfDevices > 1)
                {
                    _numHats = 1;
                    _numAxes = numberOfDevices - 2;
                }
                else
                {
                    _numAxes = numberOfDevices;
                }

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

//        meta->fileDescriptor = ((JoystickMeta*)joystick._native)->fileDescriptor;
        ///We don't want to open a new file handle every time a copy is made
        ///If PollState is called on the copy, a new handle will be created then
        meta->fileDescriptor = -1;

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

        if (meta->fileDescriptor > -1)
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

//        meta->fileDescriptor = ((JoystickMeta*)joystick._native)->fileDescriptor;

        ///We don't want to create a new file handle every time a copy is made
        ///If PollState is called on the copy, a file handle will be created then
        meta->fileDescriptor = -1;

        return *this;
    }


    void Joystick::PollState()
    {
        JoystickMeta* meta = reinterpret_cast<JoystickMeta*>(_native);

        //If the file is no longer open, try opening it again first
        if (meta->fileDescriptor < 0)
        {
            meta->fileDescriptor = OpenJoystickFileDescriptor(_numJoystick);

        }

        if (meta->fileDescriptor > -1)
        {
            js_event e;

            while (read(meta->fileDescriptor, &e, sizeof(js_event)) > 0)
            {
                switch (e.type)
                {
                    case JS_EVENT_BUTTON | JS_EVENT_INIT:
                    case JS_EVENT_BUTTON:
                    {
                        _buttonStates[e.number] = e.value == 1;
                        break;
                    }


                    case JS_EVENT_AXIS | JS_EVENT_INIT:
                    case JS_EVENT_AXIS:
                    {
                        if (_numHats == 1 && e.number >= _numAxes - 2)
                        {
                            int numHatAxis = e.number - _numAxes;
                            JoystickMeta* meta = reinterpret_cast<JoystickMeta*>(_native);
                            meta->hatState[numHatAxis] = e.value;

                            if (meta->hatState[0] < 0)
                            {
                                if (meta->hatState[1] < 0)
                                {
                                    _hatStates[0] = 31500;
                                }
                                else if (meta->hatState[1] > 0)
                                {
                                    _hatStates[0] = 22500;
                                }
                                else
                                {
                                    _hatStates[0] = 27000;
                                }
                            }
                            else if (meta->hatState[0] > 0)
                            {
                                if (meta->hatState[1] < 0)
                                {
                                    _hatStates[0] = 4500;
                                }
                                else if (meta->hatState[1] > 0)
                                {
                                    _hatStates[0] = 13500;
                                }
                                else
                                {
                                    _hatStates[0] = 9000;
                                }
                            }
                            else
                            {
                                if (meta->hatState[1] < 0)
                                {
                                    _hatStates[0] = 0;
                                }
                                else if (meta->hatState[1] > 0)
                                {
                                    _hatStates[0] = 18000;
                                }
                                else
                                {
                                    _hatStates[0] = -1;
                                }
                            }
                        }
                        else
                        {
                            _axisStates[e.number] = e.value;
                        }
                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            }

            if (errno != EAGAIN)
            {
                //perror("something went wrong");
                errno = 0;
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
