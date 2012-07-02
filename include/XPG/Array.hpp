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

            T& operator[](size_t index);
            const T& operator[](size_t index) const;

            inline size_t Size() const { return _size; }
            inline size_t Capacity() const { return _capacity; }

            void Add(const T& value);
            void Reserve(size_t capacity);

        private:
            void Copy(const Array& array);
            void Destroy();
            void Expand();

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
    T& Array<T>::operator[](size_t index)
    {
        T* target = static_cast<T*>(_block) + index;
        return *target;
    }

    template<typename T>
    const T& Array<T>::operator[](size_t index) const
    {
        const T* target = static_cast<const T*>(_block) + index;
        return *target;
    }

    template<typename T>
    void Array<T>::Add(const T& value)
    {
        if (_size == _capacity)
            Expand();

        T* target = static_cast<T*>(_block) + _size;
        new (target) T(value);

        ++_size;
    }

    template<typename T>
    void Array<T>::Copy(const Array& array)
    {
        _block = 0;
        _size = array._size;
        _capacity = _size;

        if (_size)
        {
            _block = malloc(_size * sizeof(T));

            T* a = static_cast<T*>(_block);
            const T* b = static_cast<const T*>(array._block);

            for (size_t i = 0; i < _size; ++i)
            {
                new (a++) T(*b++);
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
    void Array<T>::Expand()
    {
        if (_capacity < 1)
            _capacity = 1;
        else
            _capacity *= 2;

        void* block = malloc(_capacity);
        memcpy(block, _block, _size * sizeof(T));
        free(_block);
        _block = block;
    }
}

#endif
