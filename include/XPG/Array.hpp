#ifndef XpgArrayHpp
#define XpgArrayHpp

#include <cstdlib>
#include <cstring>

namespace XPG
{
    template<typename T>
    class Array
    {
        public:
            Array();
            Array(const Array& array);
            ~Array();

            inline operator T*()
            {
                return static_cast<T*>(_block);
            }

            inline operator const T*() const
            {
                return static_cast<T*>(_block);
            }

            inline size_t Size() const { return _size; }
            inline size_t Capacity() const { return _capacity; }

            Array& operator=(const Array& array);

            bool Add(const T& value);
            bool Reserve(size_t capacity);

        private:
            void Copy(const Array& array);
            void Destroy();
            bool Expand();

            void* _block;
            size_t _capacity;
            size_t _size;
    };

    template<typename T>
    Array<T>::Array()
    {
        _block = 0;
        _capacity = 0;
        _size = 0;
    }

    template<typename T>
    Array<T>::Array(const Array& array)
    {
        Copy(array);
    }

    template<typename T>
    Array<T>::~Array()
    {
        Destroy();
    }

    template<typename T>
    Array<T>& Array<T>::operator=(const Array& array)
    {
        Destroy();
        Copy(array);
        return *this;
    }

    template<typename T>
    bool Array<T>::Add(const T& value)
    {
        bool success = false;

        if (_size < _capacity || Expand())
        {
            T* target = static_cast<T*>(_block) + _size++;
            new (target) T(value);
            success = true;
        }

        return success;
    }

    template<typename T>
    bool Array<T>::Reserve(size_t capacity)
    {
        bool success = true;

        if (capacity > _capacity)
        {
            _capacity = capacity;

            void* block = malloc(_capacity);

            if (block)
            {
                memcpy(block, _block, _size * sizeof(T));
                free(_block);
                _block = block;
            }
            else
            {
                success = false;
            }
        }

        return success;
    }

    template<typename T>
    void Array<T>::Copy(const Array& array)
    {
        _block = 0;
        _size = 0;
        _capacity = 0;

        if (array._size)
        {
            _block = malloc(array._size * sizeof(T));

            if (_block)
            {
                _size = array._size;
                _capacity = _size;

                T* a = static_cast<T*>(_block);
                const T* b = static_cast<const T*>(array._block);

                for (size_t i = 0; i < _size; ++i)
                {
                    new (a++) T(*b++);
                }
            }
        }
    }

    template<typename T>
    void Array<T>::Destroy()
    {
        T* value = (T*)_block;

        for (size_t i = 0; i < _size; ++i)
        {
            value->~T();
            ++value;
        }

        free(_block);
    }

    template<typename T>
    bool Array<T>::Expand()
    {
        return Reserve(_capacity < 1 ? 1 : _capacity * 2);
    }
}

#endif
