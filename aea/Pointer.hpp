#ifndef POINTER_HPP
#define POINTER_HPP

#include "BasicContainer.hpp"



namespace aea
{
    template<typename T>
    class Pointer : public BasicContainer<T>
    {
        public:
            Pointer() = default;
            explicit Pointer(void* ptr);
            Pointer(void* ptr, const std::uint64_t& count);
            Pointer(const std::initializer_list<T>& list);
            Pointer(const Pointer<T>& ptr);
            Pointer(Pointer<T>&& ptr);
            virtual ~Pointer() = default;

            Pointer<T>& operator=(const Pointer<T>& ptr);
            Pointer<T>& operator=(Pointer<T>&& ptr);


        protected:
            virtual std::ostream& print(std::ostream& os) const;
    };



    template<typename T>
    Pointer<T>::Pointer(void* ptr)
    {
        this->begin = static_cast<T*>(ptr);
        this->end = (this->begin);
    }


    template<typename T>
    Pointer<T>::Pointer(void* ptr, const std::uint64_t& count)
    {
        this->begin = static_cast<T*>(ptr);
        this->end = (this->begin + count - 1);
    }


    template<typename T>
    Pointer<T>::Pointer(const std::initializer_list<T>& list) : BasicContainer<T>(list)
    {

    }


    template<typename T>
    Pointer<T>::Pointer(const Pointer<T>& ptr) : BasicContainer<T>(ptr)
    {

    }


    template<typename T>
    Pointer<T>::Pointer(Pointer<T>&& ptr) : BasicContainer<T>(std::move(ptr))
    {

    }


    template<typename T>
    Pointer<T>& Pointer<T>::operator=(const Pointer<T>& ptr)
    {
        this->reset();

        const std::uint64_t size = ptr.size();

        if (size > 0)
        {
            if (size == 1) { this->begin = new T; }
            else if (size > 1) { this->begin = new T[size]; }
            this->end = (this->begin + size - 1);

            for (std::uint64_t i = 0; i < size; ++i)
            {
                this->begin[i] = ptr.begin[i];
            }
        }

        return *this;
    }


    template<typename T>
    Pointer<T>& Pointer<T>::operator=(Pointer<T>&& ptr)
    {
        this->reset();

        this->begin = ptr.begin;
        this->end = ptr.end;

        ptr.begin = nullptr;
        ptr.end = nullptr;

        return *this;
    }


    template<typename T>
    std::ostream& Pointer<T>::print(std::ostream& os) const
    {
        return os;
    }
}



#endif  // POINTER_HPP
