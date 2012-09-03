#ifndef XpgScreenHpp
#define XpgScreenHpp

#include "Platform.hpp"
#include "DataTypes.hpp"
#include "Event.hpp"

namespace XPG
{
    XpgClass Screen
    {
        public:
            Screen();
            ~Screen();

            void Open();
            void Close();

            void Run();
            void Draw();
            void SetTitle(const char* title);
            void MakeCurrent();
            void SwapBuffers();
            
            inline Event& OnLoad() { return _onLoad; }
            inline Event& OnUnload() { return _onUnload; }
            inline Event& OnLoop() { return _onLoop; }

            inline Event& OnResize() { return _onResize; }
            inline Event& OnMove() { return _onMove; }
            inline Event& OnMinimize() { return _onMinimize; }
            inline Event& OnMaximize() { return _onMaximize; }
            inline Event& OnRestore() { return _onRestore; }
            inline Event& OnOpen() { return _onOpen; }
            inline Event& OnClose() { return _onClose; }
            inline Event& OnFocus() { return _onFocus; }
            inline Event& OnBlur() { return _onBlur; }
            inline Event& OnExpose() { return _onExpose; }

            inline Event& OnKeyDown() { return _onKeyDown; }
            inline Event& OnKeyRepeat() { return _onKeyRepeat; }
            inline Event& OnKeyUp() { return _onKeyUp; }

            inline Event& OnMouseMove() { return _onMouseMove; }
            inline Event& OnMouseIn() { return _onMouseIn; }
            inline Event& OnMouseOut() { return _onMouseOut; }
            inline Event& OnMouseButtonDown() { return _onMouseButtonDown; }
            inline Event& OnMouseButtonUp() { return _onMouseButtonUp; }
            inline Event& OnMouseWheelDown() { return _onMouseWheelDown; }
            inline Event& OnMouseWheelUp() { return _onMouseWheelUp; }

        private:
            UInt8 _native[256];
            
            Event _onLoad;
            Event _onUnload;
            Event _onLoop;

            Event _onResize;
            Event _onMove;
            Event _onMinimize;
            Event _onMaximize;
            Event _onRestore;
            Event _onOpen;
            Event _onClose;
            Event _onFocus;
            Event _onBlur;
            Event _onExpose;

            Event _onKeyDown;
            Event _onKeyRepeat;
            Event _onKeyUp;

            Event _onMouseMove;
            Event _onMouseIn;
            Event _onMouseOut;
            Event _onMouseButtonDown;
            Event _onMouseButtonUp;
            Event _onMouseWheelDown;
            Event _onMouseWheelUp;
    };
}

#endif
