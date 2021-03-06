#ifndef BASICCONTAINER_HPP
#define BASICCONTAINER_HPP

#include "Functions.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string.h>
#include <string>
#include <cinttypes>
#include <typeinfo>
#include <type_traits>
#include <exception>
#include <stdexcept>
#include <initializer_list>



namespace aea
{
    template<typename T>
    class BasicContainer
    {
        public:
            BasicContainer() = default;
            BasicContainer(const std::initializer_list<T>& list);
            BasicContainer(const BasicContainer<T>& obj);
            BasicContainer(BasicContainer<T>&& obj);
            virtual ~BasicContainer();

            BasicContainer<T>& operator=(const BasicContainer<T>& obj);
            BasicContainer<T>& operator=(BasicContainer<T>&& obj);

            virtual std::uint64_t size() const;
            virtual T* get() const;
            T* first() const;
            T* last() const;
            virtual T& at(const std::uint64_t& position) const;
            virtual T& operator[](const std::uint64_t& position) const;
            virtual void reset();


        protected:
            virtual T* getAllocatedMemory(const std::uint64_t& size);
            virtual std::ostream& print(std::ostream& os) const = 0;


        protected:
            T* begin = nullptr;
            T* end = nullptr;
    };



    template<typename T>
    BasicContainer<T>::BasicContainer(const std::initializer_list<T>& list)
    {
        const std::uint64_t size = list.size();

        if (size > 0)
        {
            if (size == 1) { begin = new T; }
            else if (size > 1) { begin = new T[size]; }

            end = (begin + size - 1);

            T* beginTemp = begin;
            for (const T& item : list)
            {
                *beginTemp = item;
                beginTemp += 1;
            }
        }
    }


    template<typename T>
    BasicContainer<T>::BasicContainer(const BasicContainer<T>& obj)
    {
        const std::uint64_t size = obj.size();

        if (size > 0)
        {
            if (size == 1) { begin = new T; }
            else if (size > 1) { begin = new T[size]; }
            end = (begin + size - 1);

            for (std::uint64_t i = 0; i < size; ++i)
            {
                begin[i] = obj.begin[i];
            }
        }
    }


    template<typename T>
    BasicContainer<T>::BasicContainer(BasicContainer<T>&& obj)
    {
        begin = obj.begin;
        end = obj.end;

        obj.begin = nullptr;
        obj.end = nullptr;
    }



    template<typename T>
    BasicContainer<T>::~BasicContainer()
    {
        if (size() == 1)
        {
            delete begin;
        }

        else if (size() > 1)
        {
            delete [] begin;
        }
    }


    template<typename T>
    BasicContainer<T>& BasicContainer<T>::operator=(const BasicContainer<T>& obj)
    {
        if (obj.begin != nullptr)
        {
            reset();

            const std::uint64_t size = obj.size();

            if (size > 0)
            {
                if (size == 1) { begin = new T; }
                else if (size > 1) { begin = new T[size]; }
                end = (begin + size - 1);

                aea::arrcopy(begin, obj.begin, size);
            }
        }

        return *this;
    }


    template<typename T>
    BasicContainer<T>& BasicContainer<T>::operator=(BasicContainer<T>&& obj)
    {
        reset();

        begin = obj.begin;
        end = obj.end;

        obj.begin = nullptr;
        obj.end = nullptr;

        return *this;
    }


    template<typename T>
    std::uint64_t BasicContainer<T>::size() const
    {
        if (begin != nullptr)
        {
            return (std::uint64_t(end - begin) + 1);
        }

        return 0;
    }


    template<typename T>
    T* BasicContainer<T>::get() const
    {
        return begin;
    }


    template<typename T>
    T* BasicContainer<T>::first() const
    {
        return begin;
    }


    template<typename T>
    T* BasicContainer<T>::last() const
    {
        if (end != nullptr)
        {
            return end;
        }

        return nullptr;
    }


    template<typename T>
    T& BasicContainer<T>::at(const std::uint64_t& position) const
    {
        if (begin == nullptr || position >= size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        return begin[position];
    }


    template<typename T>
    T& BasicContainer<T>::operator[](const std::uint64_t& position) const
    {
        if (begin == nullptr || position >= size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        return begin[position];
    }


    template<typename T>
    void BasicContainer<T>::reset()
    {
        if (begin != nullptr)
        {
            if (size() == 1)
            {
                delete begin;
            }

            else if (size() > 1)
            {
                delete [] begin;
            }
            
            begin = nullptr;
            end = nullptr;
        }
    }


    template<typename T>
    T* BasicContainer<T>::getAllocatedMemory(const std::uint64_t& size)
    {
        T* newData = nullptr;

        if (size == 1) { newData = new T; }
        else if (size > 1) { newData = new T[size]; }

        return newData;
    }


    template<typename Type> 
    Type* iterate_front(Type** accesser, const BasicContainer<Type>& obj, const Type* const start, const std::uint64_t& by = 1)
    {
        if (accesser == nullptr || accesser[0] >= obj.last())
        {
            return nullptr;
        }

        else if (accesser[0] == nullptr)
        {
            accesser[0] = (Type*) start;
        }

        else
        {
            accesser[0] += by;
        }

        return accesser[0];
    }


    template<typename Type> 
    Type* iterate_back(Type** accesser, const BasicContainer<Type>& obj, const Type* const start, const std::uint64_t& by = 1)
    {
        if (accesser == nullptr || accesser[0] <= obj.first())
        {
            return nullptr;
        }

        else if (accesser[0] == nullptr)
        {
            accesser[0] = (Type*) start;
        }

        else
        {
            accesser[0] -= by;
        }

        return accesser[0];
    }
}



#endif  // BASICCONTAINER_HPP
