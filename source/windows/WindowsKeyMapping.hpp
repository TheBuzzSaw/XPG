#ifndef XpgWindowsKeyMapping
#define XpgWindowsKeyMapping

#include "..\..\include\XPG\Key.hpp"

namespace XPG
{
    struct KeyMapping
    {
        Key::Code normal;
        Key::Code extended;
    };

    const size_t NumMappings = 93;
    const KeyMapping KeyTable[NumMappings] = {
        { Key::Unknown, Key::Unknown },                 // 0
        { Key::Escape, Key::Escape },                   // 1
        { Key::TopRow1, Key::TopRow1 },                 // 2
        { Key::TopRow2, Key::TopRow2 },                 // 3
        { Key::TopRow3, Key::TopRow3 },                 // 4
        { Key::TopRow4, Key::TopRow4 },                 // 5
        { Key::TopRow5, Key::TopRow5 },                 // 6
        { Key::TopRow6, Key::TopRow6 },                 // 7
        { Key::TopRow7, Key::TopRow7 },                 // 8
        { Key::TopRow8, Key::TopRow8 },                 // 9
        { Key::TopRow9, Key::TopRow9 },                 // 10
        { Key::TopRow0, Key::TopRow0 },                 // 11
        { Key::Minus, Key::Minus },                     // 12
        { Key::Equals, Key::Equals },                   // 13
        { Key::Backspace, Key::Backspace },             // 14
        { Key::Tab, Key::Tab },                         // 15
        { Key::LetterQ, Key::LetterQ },                 // 16
        { Key::LetterW, Key::LetterW },                 // 17
        { Key::LetterE, Key::LetterE },                 // 18
        { Key::LetterR, Key::LetterR },                 // 19
        { Key::LetterT, Key::LetterT },                 // 20
        { Key::LetterY, Key::LetterY },                 // 21
        { Key::LetterU, Key::LetterU },                 // 22
        { Key::LetterI, Key::LetterI },                 // 23
        { Key::LetterO, Key::LetterO },                 // 24
        { Key::LetterP, Key::LetterP },                 // 25
        { Key::LeftBracket, Key::LeftBracket },         // 26
        { Key::RightBracket, Key::RightBracket },       // 27
        { Key::Enter, Key::KeypadEnter },               // 28
        { Key::LeftControl, Key::RightControl },        // 29
        { Key::LetterA, Key::LetterA },                 // 30
        { Key::LetterS, Key::LetterS },                 // 31
        { Key::LetterD, Key::LetterD },                 // 32
        { Key::LetterF, Key::LetterF },                 // 33
        { Key::LetterG, Key::LetterG },                 // 34
        { Key::LetterH, Key::LetterH },                 // 35
        { Key::LetterJ, Key::LetterJ },                 // 36
        { Key::LetterK, Key::LetterK },                 // 37
        { Key::LetterL, Key::LetterL },                 // 38
        { Key::Semicolon, Key::Semicolon },             // 39
        { Key::Quote, Key::Quote },                     // 40
        { Key::BackQuote, Key::BackQuote },             // 41
        { Key::LeftShift, Key::LeftShift },             // 42
        { Key::Backslash, Key::Backslash },             // 43
        { Key::LetterZ, Key::LetterZ },                 // 44
        { Key::LetterX, Key::LetterX },                 // 45
        { Key::LetterC, Key::LetterC },                 // 46
        { Key::LetterV, Key::LetterV },                 // 47
        { Key::LetterB, Key::LetterB },                 // 48
        { Key::LetterN, Key::LetterN },                 // 49
        { Key::LetterM, Key::LetterM },                 // 50
        { Key::Comma, Key::Comma },                     // 51
        { Key::Period, Key::Period },                   // 52
        { Key::Slash, Key::KeypadSlash },               // 53
        { Key::RightShift, Key::RightShift },           // 54
        { Key::KeypadAsterisk, Key::KeypadAsterisk },   // 55
        { Key::LeftAlt, Key::RightAlt },                // 56
        { Key::Space, Key::Space },                     // 57
        { Key::CapsLock, Key::CapsLock },               // 58
        { Key::F1, Key::F1 },                           // 59
        { Key::F2, Key::F2 },                           // 60
        { Key::F3, Key::F3 },                           // 61
        { Key::F4, Key::F4 },                           // 62
        { Key::F5, Key::F5 },                           // 63
        { Key::F6, Key::F6 },                           // 64
        { Key::F7, Key::F7 },                           // 65
        { Key::F8, Key::F8 },                           // 66
        { Key::F9, Key::F9 },                           // 67
        { Key::F10, Key::F10 },                         // 68
        { Key::Pause, Key::NumLock },                   // 69
        { Key::ScrollLock, Key::ScrollLock },           // 70
        { Key::Keypad7, Key::Home },                    // 71
        { Key::Keypad8, Key::Up },                      // 72
        { Key::Keypad9, Key::PageUp },                  // 73
        { Key::KeypadSlash, Key::KeypadSlash },         // 74
        { Key::Keypad4, Key::Left },                    // 75
        { Key::Keypad5, Key::Keypad5 },                 // 76
        { Key::Keypad6, Key::Right },                   // 77
        { Key::KeypadPlus, Key::KeypadPlus },           // 78
        { Key::Keypad1, Key::End },                     // 79
        { Key::Keypad2, Key::Down },                    // 80
        { Key::Keypad3, Key::PageDown },                // 81
        { Key::Keypad0, Key::Insert },                  // 82
        { Key::KeypadPeriod, Key::Delete },             // 83
        { Key::Unknown, Key::Unknown },                 // 84
        { Key::Unknown, Key::Unknown },                 // 85
        { Key::Unknown, Key::Unknown },                 // 86
        { Key::F11, Key::F11 },                         // 87
        { Key::F12, Key::F12 },                         // 88
        { Key::Unknown, Key::Unknown },                 // 89
        { Key::Unknown, Key::Unknown },                 // 90
        { Key::LeftSuper, Key::LeftSuper },             // 91
        { Key::RightSuper, Key::RightSuper },           // 92
    };

    inline Key::Code lookupKey(unsigned int inCode, bool inExtended)
    {
        return inCode < NumMappings ?
            (inExtended ? KeyTable[inCode].extended : KeyTable[inCode].normal)
            : Key::Unknown;
    }
}

#endif
