#ifndef XpgXHpp
#define XpgXHpp

#include "../../include/XPG/Window.hpp"
#include "../../include/XPG/Key.hpp"
#include "../../include/XPG/OpenGL/glew.h"
#include "../../include/XPG/OpenGL/glxew.h"
#include "../EventBatch.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <map>

namespace XPG
{
    struct WindowMeta
    {
        GLXContext context;
        ::Window window;
        Atom wmDeleteMessage;
        Window* object;
        EventBatch events;
    };

    struct ApplicationMeta
    {
        Display* display;
        Atom wmDeleteMessage;
        size_t windowCount;
        size_t referenceCount;
    };

    extern ApplicationMeta* theApplicationMeta;
    extern std::map< ::Window, WindowMeta*> windows;

    void SetGlxFunctionPointers();
    void ProcessEvent(const XEvent& event);

    const size_t NumMappings = 135;
    const Key::Code KeyTable[NumMappings] = {
        Key::Unknown, // 0
        Key::Unknown, // 1
        Key::Unknown, // 2
        Key::Unknown, // 3
        Key::Unknown, // 4
        Key::Unknown, // 5
        Key::Unknown, // 6
        Key::Unknown, // 7
        Key::Unknown, // 8
        Key::Escape, // 9
        Key::TopRow1, // 10
        Key::TopRow2, // 11
        Key::TopRow3, // 12
        Key::TopRow4, // 13
        Key::TopRow5, // 14
        Key::TopRow6, // 15
        Key::TopRow7, // 16
        Key::TopRow8, // 17
        Key::TopRow9, // 18
        Key::TopRow0, // 19
        Key::Minus, // 20
        Key::Equals, // 21
        Key::Backspace, // 22
        Key::Tab, // 23
        Key::LetterQ, // 24
        Key::LetterW, // 25
        Key::LetterE, // 26
        Key::LetterR, // 27
        Key::LetterT, // 28
        Key::LetterY, // 29
        Key::LetterU, // 30
        Key::LetterI, // 31
        Key::LetterO, // 32
        Key::LetterP, // 33
        Key::LeftBracket, // 34
        Key::RightBracket, // 35
        Key::Enter, // 36
        Key::LeftControl, // 37
        Key::LetterA, // 38
        Key::LetterS, // 39
        Key::LetterD, // 40
        Key::LetterF, // 41
        Key::LetterG, // 42
        Key::LetterH, // 43
        Key::LetterJ, // 44
        Key::LetterK, // 45
        Key::LetterL, // 46
        Key::Semicolon, // 47
        Key::Quote, // 48
        Key::BackQuote, // 49
        Key::LeftShift, // 50
        Key::Backslash, // 51
        Key::LetterZ, // 52
        Key::LetterX, // 53
        Key::LetterC, // 54
        Key::LetterV, // 55
        Key::LetterB, // 56
        Key::LetterN, // 57
        Key::LetterM, // 58
        Key::Comma, // 59
        Key::Period, // 60
        Key::Slash, // 61
        Key::RightShift, // 62
        Key::KeypadAsterisk, // 63
        Key::LeftAlt, // 64
        Key::Space, // 65
        Key::CapsLock, // 66
        Key::F1, // 67
        Key::F2, // 68
        Key::F3, // 69
        Key::F4, // 70
        Key::F5, // 71
        Key::F6, // 72
        Key::F7, // 73
        Key::F8, // 74
        Key::F9, // 75
        Key::F10, // 76
        Key::NumLock, // 77
        Key::ScrollLock, // 78
        Key::Keypad7, // 79
        Key::Keypad8, // 80
        Key::Keypad9, // 81
        Key::KeypadMinus, // 82
        Key::Keypad4, // 83
        Key::Keypad5, // 84
        Key::Keypad6, // 85
        Key::KeypadPlus, // 86
        Key::Keypad1, // 87
        Key::Keypad2, // 88
        Key::Keypad3, // 89
        Key::Keypad0, // 90
        Key::KeypadPeriod, // 91
        Key::Unknown, // 92
        Key::Unknown, // 93
        Key::Unknown, // 94
        Key::F11, // 95
        Key::F12, // 96
        Key::Unknown, // 97
        Key::Unknown, // 98
        Key::Unknown, // 99
        Key::Unknown, // 100
        Key::Unknown, // 101
        Key::Unknown, // 102
        Key::Unknown, // 103
        Key::KeypadEnter, // 104
        Key::RightControl, // 105
        Key::KeypadSlash, // 106
        Key::Unknown, // 107
        Key::RightAlt, // 108
        Key::Unknown, // 109
        Key::Home, // 110
        Key::Up, // 111
        Key::PageUp, // 112
        Key::Left, // 113
        Key::Right, // 114
        Key::End, // 115
        Key::Down, // 116
        Key::PageDown, // 117
        Key::Insert, // 118
        Key::Delete, // 119
        Key::Unknown, // 120
        Key::Unknown, // 121
        Key::Unknown, // 122
        Key::Unknown, // 123
        Key::Unknown, // 124
        Key::Unknown, // 125
        Key::Unknown, // 126
        Key::Pause, // 127
        Key::Unknown, // 128
        Key::Unknown, // 129
        Key::Unknown, // 130
        Key::Unknown, // 131
        Key::Unknown, // 132
        Key::LeftSuper, // 133
        Key::RightSuper, // 134
        };

    inline Key::Code LookupKey(size_t id)
    {
        return id < NumMappings ? KeyTable[id] : Key::Unknown;
    }
}

#endif
