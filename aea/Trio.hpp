#ifndef TRIO_H
#define TRIO_H
#include "Duo.hpp"



namespace aea
{
    template<typename T, typename T2, typename T3>
    class Trio : public Duo<T, T2>
    {
        public:
            Trio() = default;
            Trio(const T& item, const T2& item2, const T3& item3);
            Trio(const Trio<T, T2, T3>& obj);
            Trio(Trio<T, T2, T3>&& obj);
            virtual ~Trio();

            Trio<T, T2, T3>& operator=(const Trio<T, T2, T3>& obj);
            Trio<T, T2, T3>& operator=(Trio<T, T2, T3>&& obj);

            virtual void reset();
            T3* third() const;

        
        private:
            T3* three = nullptr;
    };



    template<typename T, typename T2, typename T3>
    Trio<T, T2, T3>::Trio(const T& item, const T2& item2, const T3& item3) : Duo<T, T2>(item, item2)
    {
        three = new T3(item3);
    }


    template<typename T, typename T2, typename T3>
    Trio<T, T2, T3>::Trio(const Trio<T, T2, T3>& obj) : Duo<T, T2>(obj)
    {
        three = new T3(*obj.three);
    }


    template<typename T, typename T2, typename T3>
    Trio<T, T2, T3>::Trio(Trio<T, T2, T3>&& obj) : Duo<T, T2>(std::move(obj))
    {
        three = obj.three;
        obj.three = nullptr;
    }


    template<typename T, typename T2, typename T3>
    Trio<T, T2, T3>::~Trio()
    {
        delete three;
    }


    template<typename T, typename T2, typename T3>
    Trio<T, T2, T3>& Trio<T, T2, T3>::operator=(const Trio<T, T2, T3>& obj)
    {
        Duo<T, T2>* temp = static_cast<Duo<T, T2>*>(this);
        *temp = obj;

        three = new T3(*obj.three);

        return *this;
    }


    template<typename T, typename T2, typename T3>
    Trio<T, T2, T3>& Trio<T, T2, T3>::operator=(Trio<T, T2, T3>&& obj)
    {
        Duo<T, T2>* temp = static_cast<Duo<T, T2>*>(this);
        *temp = std::move(obj);

        three = obj.three;
        obj.three = nullptr;

        return *this;
    }


    template<typename T, typename T2, typename T3>
    void Trio<T, T2, T3>::reset()
    {
        if (this->one != nullptr)
        {
            delete this->one;
            this->one = nullptr;
        }

        if (this->two != nullptr)
        {
            delete this->two;
            this->two = nullptr;
        }

        if (three != nullptr)
        {
            delete three;
            three = nullptr;
        }
    }


    template<typename T, typename T2, typename T3>
    T3* Trio<T, T2, T3>::third() const
    {
        return three;
    }
}



#endif  // TRIO_H
