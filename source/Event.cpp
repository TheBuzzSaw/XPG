#include "../include/XPG/Event.hpp"

namespace XPG
{
    Event::Event()
    {
        _callback = 0;
        _userData = 0;
    }

    Event::Event(const Event& other)
    {
        _callback = other._callback;
        _userData = other._userData;
    }

    Event::~Event()
    {
    }

    Event& Event::operator=(const Event& other)
    {
        _callback = other._callback;
        _userData = other._userData;
        return *this;
    }

    void Event::Aim(Event::Callback callback, void* userData)
    {
        _callback = callback;
        _userData = userData;
    }

    void Event::Clear()
    {
        _callback = 0;
        _userData = 0;
    }

    void Event::Fire(Event::Details& details) const
    {
        if (_callback)
        {
            details.userData = _userData;
            _callback(details);
        }
    }
}
