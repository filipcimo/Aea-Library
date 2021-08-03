#ifndef DUO_HPP
#define DUO_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>



namespace aea
{
    template<typename T, typename T2>
    class Duo
    {
        public:
            Duo() = default;
            Duo(const T& item, const T2& item2);
            Duo(T&& item, T2&& item2);
            Duo(const Duo<T, T2>& obj);
            Duo(Duo<T, T2>&& obj);
            virtual ~Duo();

            Duo<T, T2>& operator=(const Duo<T, T2>& obj);
            Duo<T, T2>& operator=(Duo<T, T2>&& obj);

            virtual void reset() override;
            T* first() const;
            T2* second() const;

        
        protected:
            T* one = nullptr;
            T2* two = nullptr;
    };



    template<typename T, typename T2>
    Duo<T, T2>::Duo(const T& item, const T2& item2)
    {
        one = new T(item);
        two = new T2(item2);
    }


    template<typename T, typename T2>
    Duo<T, T2>::Duo(T&& item, T2&& item2)
    {
        one = new T(std::move(item));
        two = new T2(std::move(item2));
    }


    template<typename T, typename T2>
    Duo<T, T2>::Duo(const Duo<T, T2>& obj)
    {
        one = new T(*obj.one);
        two = new T2(*obj.two);
    }


    template<typename T, typename T2>
    Duo<T, T2>::Duo(Duo<T, T2>&& obj)
    {
        one = obj.one;
        two = obj.two;

        obj.one = nullptr;
        obj.two = nullptr;
    }


    template<typename T, typename T2>
    Duo<T, T2>::~Duo()
    {
        if (one != nullptr)
        {
            delete one;
        }

        if (two != nullptr)
        {
            delete two;
        }
    }


    template<typename T, typename T2>
    Duo<T, T2>& Duo<T, T2>::operator=(const Duo<T, T2>& obj)
    {
        reset();

        one = new T(*obj.one);
        two = new T2(*obj.two);

        return *this;
    }


    template<typename T, typename T2>
    Duo<T, T2>& Duo<T, T2>::operator=(Duo<T, T2>&& obj)
    {
        reset();

        one = obj.one;
        two = obj.two;

        obj.one = nullptr;
        obj.two = nullptr;

        return *this;
    }


    template<typename T, typename T2>
    void Duo<T, T2>::reset()
    {
        if (one != nullptr)
        {
            delete one;
            one = nullptr;
        }

        if (two != nullptr)
        {
            delete two;
            two = nullptr;
        }
    }


    template<typename T, typename T2>
    T* Duo<T, T2>::first() const
    {
        return one;
    }


    template<typename T, typename T2>
    T2* Duo<T, T2>::second() const
    {
        return two;
    }
}



#endif  // DUO_HPP