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
            virtual T& at(const std::uint64_t& position) const;
            virtual T& operator[](const std::uint64_t& position) const;
            virtual void reset();


        protected:
            virtual std::ostream& print(std::ostream& os) const = 0;


        protected:
            T* begin = nullptr;
            T* end = nullptr;
    };



    template<typename T>
    BasicContainer<T>::BasicContainer(const std::initializer_list<T>& list)
    {
        const std::uint64_t size = list.size();

        begin = new T[size];
        end = (begin + size);

        for (std::uint64_t i = 0; i < size; ++i)
        {
            begin[i] = *(list.begin() + i);
        }
    }


    template<typename T>
    BasicContainer<T>::BasicContainer(const BasicContainer<T>& obj)
    {
        const std::uint64_t size = (obj.end - obj.begin);

        begin = new T[size];
        end = (begin + size);

        for (std::uint64_t i = 0; i < size; ++i)
        {
            begin[i] = obj.begin[i];
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
        if (begin != nullptr)
        {
            if ((end - begin) == 1)
            {
                delete begin;
            }

            else if ((end - begin) > 1)
            {
                delete [] begin;
            }
        }
    }


    template<typename T>
    BasicContainer<T>& BasicContainer<T>::operator=(const BasicContainer<T>& obj)
    {
        if (obj.begin != nullptr)
        {
            reset();

            const std::uint64_t size = obj.size();

            begin = new T[size];
            end = (begin + size);

            aea::arrcpy(begin, obj.begin, size);
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
            return (end - begin);
        }

        return 0;
    }


    template<typename T>
    T* BasicContainer<T>::get() const
    {
        return begin;
    }



    template<typename T>
    T& BasicContainer<T>::at(const std::uint64_t& position) const
    {
        if (begin == nullptr || position >= (end - begin))
        {
 
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        return begin[position];
    }


    template<typename T>
    T& BasicContainer<T>::operator[](const std::uint64_t& position) const
    {
        if (begin == nullptr || position >= (end - begin))
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
            if ((end - begin) == 1)
            {
                delete begin;
            }

            else if ((end - begin) > 1)
            {
                delete [] begin;
            }
            
            begin = nullptr;
            end = nullptr;
        }
    }
}



#endif  // BASICCONTAINER_H
