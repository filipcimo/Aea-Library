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


            template<typename Type> friend std::ostream& operator<<(std::ostream& os, const Pointer<Type>& obj);


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
    Pointer<T>::Pointer(Pointer<T>&& ptr) : BasicContainer<T>(ptr)
    {

    }


    template<typename T>
    Pointer<T>& Pointer<T>::operator=(const Pointer<T>& ptr)
    {
        this->reset();

        const std::uint64_t size = ptr.size();

        this->begin = new T[size];

        for (std::uint64_t i = 0; i < size; ++i)
        {
            this->begin[i] = ptr.begin[i];
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
        os.flush();

        for (std::uint64_t i = 0; i < this->size(); ++i)
        {
            os << this->begin[i] << ' ';
        }

        return os;
    }


    template<typename Type>
    std::ostream& operator<<(std::ostream& os, const Pointer<Type>& obj)
    {
        return obj.print(os);
    }
}



#endif  // POINTER_H
