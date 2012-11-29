#ifndef XpgKeyHpp
#define XpgKeyHpp


namespace XPG
{
    namespace Key
    {
        enum Code
        {
            Unknown,

            /// letter keys
            LetterA, LetterB, LetterC, LetterD, LetterE, LetterF, LetterG,
            LetterH, LetterI, LetterJ, LetterK, LetterL, LetterM, LetterN,
            LetterO, LetterP, LetterQ, LetterR, LetterS, LetterT, LetterU,
            LetterV, LetterW, LetterX, LetterY, LetterZ,

            /// function keys
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

            /// top row numbers/symbols
            Escape, BackQuote, TopRow1, TopRow2, TopRow3, TopRow4, TopRow5,
            TopRow6, TopRow7, TopRow8, TopRow9, TopRow0, Minus, Equals,
            Backspace,

            /// symbols surrounding letter keys
            Tab, LeftBracket, RightBracket, Backslash, CapsLock, Semicolon,
            Quote, Enter, Comma, Period, Slash, Space,

            /// mod keys
            LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt,
            LeftSuper, RightSuper,

            /// control keys
            Insert, Home, PageUp, PageDown, End, Delete, Up, Down, Left, Right,
            ScrollLock, Pause, PrintScreen,

            /// keypad
            Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6,
            Keypad7, Keypad8, Keypad9, KeypadPeriod, KeypadEnter, KeypadPlus,
            KeypadMinus, KeypadAsterisk, KeypadSlash, NumLock
        };
    }
}

#endif
