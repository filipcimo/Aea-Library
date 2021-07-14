#ifndef VAR_HPP
#define VAR_HPP

#include "Pointer.hpp"
#include "sentence.hpp"

#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>



namespace aea
{
    class var
    {
        public:
            var() = default;
            template<typename T> var(const T& obj);
            template<typename T> var(T&& obj);
            var(const var& obj);
            var(var&& obj);
            ~var();

            template<typename T> var& operator=(const T& obj);
            template<typename T> var& operator=(T&& obj);
            var& operator=(const var& obj);
            var& operator=(var&& obj);
            
            template<typename T> operator T();

            void reset();
            template<typename T> void reset();
            const char* type() const;
            bool isTrivial() const;
            bool isData() const;

            template<typename T> friend T& var_cast(var& obj);
            template<typename T, typename...Args> friend var make_var(Args&&...args);


        private:
            struct Attributes
            {
                void* data = nullptr;
                std::type_index dataType = typeid(void);
                bool isTrivial = false;
            };

            Attributes* const dataObject = new Attributes();
    };


    template<typename T>
    var::var(const T& obj)
    {   
        dataObject->data = new T(obj);
        dataObject->dataType = typeid(T);
        dataObject->isTrivial = std::is_trivial<T>::value;
    }


    template<typename T>
    var::var(T&& obj)
    {   
        dataObject->data = new T(std::move(obj));
        dataObject->dataType = typeid(T);
        dataObject->isTrivial = std::is_trivial<T>::value;
    }


    var::var(const var& obj)
    {
        if (obj.dataObject != nullptr)
        {
            dataObject->dataType = obj.dataObject->dataType;
            dataObject->isTrivial = obj.dataObject->isTrivial;
            
            const char* type = dataObject->dataType.name();

            if (strcmp(type, typeid(int).name()) == 0) { dataObject->data = new int(*static_cast<int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(unsigned int).name()) == 0) { dataObject->data = new unsigned int(*static_cast<unsigned int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(short int).name()) == 0) { dataObject->data = new short int(*static_cast<short int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(unsigned short int).name()) == 0) { dataObject->data = new unsigned short int(*static_cast<unsigned short int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(long int).name()) == 0) { dataObject->data = new long int(*static_cast<long int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(unsigned long int).name()) == 0) { dataObject->data = new unsigned long int(*static_cast<unsigned long int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(long long int).name()) == 0) { dataObject->data = new long long int(*static_cast<long long int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(unsigned long long int).name()) == 0) { dataObject->data = new unsigned long long int(*static_cast<unsigned long long int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(size_t).name()) == 0) { dataObject->data = new size_t(*static_cast<size_t*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(float).name()) == 0) { dataObject->data = new float(*static_cast<float*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(double).name()) == 0) { dataObject->data = new double(*static_cast<double*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(long double).name()) == 0) { dataObject->data = new long double(*static_cast<long double*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(bool).name()) == 0) { dataObject->data = new bool(*static_cast<bool*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(char).name()) == 0) { dataObject->data = new char(*static_cast<char*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(aea::sentence).name()) == 0) { dataObject->data = new aea::sentence(*static_cast<aea::sentence*>(obj.dataObject->data)); }
        }
    }


    var::var(var&& obj)
    {
        dataObject->data = obj.dataObject->data;
        dataObject->dataType = obj.dataObject->dataType;
        dataObject->isTrivial = obj.dataObject->isTrivial;

        obj.dataObject->data = nullptr;
        obj.dataObject->dataType = typeid(void);
        obj.dataObject->isTrivial = false;
    }


    var::~var()
    {
        if (dataObject->data != nullptr)
        {
            const char* type = dataObject->dataType.name();

            if (strcmp(type, typeid(int).name()) == 0) { delete static_cast<int*>(dataObject->data); }
            else if (strcmp(type, typeid(unsigned int).name()) == 0) { delete static_cast<unsigned int*>(dataObject->data); }
            else if (strcmp(type, typeid(short int).name()) == 0) { delete static_cast<short int*>(dataObject->data); }
            else if (strcmp(type, typeid(unsigned short int).name()) == 0) { delete static_cast<unsigned short int*>(dataObject->data); }
            else if (strcmp(type, typeid(long int).name()) == 0) { delete static_cast<long int*>(dataObject->data); }
            else if (strcmp(type, typeid(unsigned long int).name()) == 0) { delete static_cast<unsigned long int*>(dataObject->data); }
            else if (strcmp(type, typeid(long long int).name()) == 0) { delete static_cast<long long int*>(dataObject->data); }
            else if (strcmp(type, typeid(unsigned long long int).name()) == 0) { delete static_cast<unsigned long long int*>(dataObject->data); }
            else if (strcmp(type, typeid(size_t).name()) == 0) { delete static_cast<size_t*>(dataObject->data); }
            else if (strcmp(type, typeid(float).name()) == 0) { delete static_cast<float*>(dataObject->data); }
            else if (strcmp(type, typeid(double).name()) == 0) { delete static_cast<double*>(dataObject->data); }
            else if (strcmp(type, typeid(long double).name()) == 0) { delete static_cast<long double*>(dataObject->data); }
            else if (strcmp(type, typeid(bool).name()) == 0) { delete static_cast<bool*>(dataObject->data); }
            else if (strcmp(type, typeid(char).name()) == 0) { delete static_cast<char*>(dataObject->data); }
            else if (strcmp(type, typeid(aea::sentence).name()) == 0) { delete static_cast<aea::sentence*>(dataObject->data); }
        }

        delete dataObject;
    }


    template<typename T>
    var& var::operator=(const T& obj)
    {   
        reset();

        dataObject->data = new T(obj);
        dataObject->dataType = typeid(T);
        dataObject->isTrivial = std::is_trivial<T>::value;

        return *this;
    }


    template<typename T>
    var& var::operator=(T&& obj)
    {   
        reset();

        dataObject->data = new T(std::move(obj));
        dataObject->dataType = typeid(T);
        dataObject->isTrivial = std::is_trivial<T>::value;

        return *this;
    }


    var& var::operator=(const var& obj)
    {
        reset();

        if (obj.dataObject != nullptr)
        {
            dataObject->dataType = obj.dataObject->dataType;
            dataObject->isTrivial = obj.dataObject->isTrivial;
            
            const char* type = dataObject->dataType.name();

            if (strcmp(type, typeid(int).name()) == 0) { dataObject->data = new int(*static_cast<int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(unsigned int).name()) == 0) { dataObject->data = new unsigned int(*static_cast<unsigned int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(short int).name()) == 0) { dataObject->data = new short int(*static_cast<short int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(unsigned short int).name()) == 0) { dataObject->data = new unsigned short int(*static_cast<unsigned short int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(long int).name()) == 0) { dataObject->data = new long int(*static_cast<long int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(unsigned long int).name()) == 0) { dataObject->data = new unsigned long int(*static_cast<unsigned long int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(long long int).name()) == 0) { dataObject->data = new long long int(*static_cast<long long int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(unsigned long long int).name()) == 0) { dataObject->data = new unsigned long long int(*static_cast<unsigned long long int*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(size_t).name()) == 0) { dataObject->data = new size_t(*static_cast<size_t*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(float).name()) == 0) { dataObject->data = new float(*static_cast<float*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(double).name()) == 0) { dataObject->data = new double(*static_cast<double*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(long double).name()) == 0) { dataObject->data = new long double(*static_cast<long double*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(bool).name()) == 0) { dataObject->data = new bool(*static_cast<bool*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(char).name()) == 0) { dataObject->data = new char(*static_cast<char*>(obj.dataObject->data)); }
            else if (strcmp(type, typeid(aea::sentence).name()) == 0) { dataObject->data = new aea::sentence(*static_cast<aea::sentence*>(obj.dataObject->data)); }
        }

        return *this;
    }


    var& var::operator=(var&& obj)
    {
        reset();

        dataObject->data = obj.dataObject->data;
        dataObject->dataType = obj.dataObject->dataType;
        dataObject->isTrivial = obj.dataObject->isTrivial;

        obj.dataObject->data = nullptr;
        obj.dataObject->dataType = typeid(void);
        obj.dataObject->isTrivial = false;

        return *this;
    }


    void var::reset()
    {
        if (dataObject->data != nullptr)
        {
            const char* type = dataObject->dataType.name();

            if (strcmp(type, typeid(int).name()) == 0) { delete static_cast<int*>(dataObject->data); }
            else if (strcmp(type, typeid(unsigned int).name()) == 0) { delete static_cast<unsigned int*>(dataObject->data); }
            else if (strcmp(type, typeid(short int).name()) == 0) { delete static_cast<short int*>(dataObject->data); }
            else if (strcmp(type, typeid(unsigned short int).name()) == 0) { delete static_cast<unsigned short int*>(dataObject->data); }
            else if (strcmp(type, typeid(long int).name()) == 0) { delete static_cast<long int*>(dataObject->data); }
            else if (strcmp(type, typeid(unsigned long int).name()) == 0) { delete static_cast<unsigned long int*>(dataObject->data); }
            else if (strcmp(type, typeid(long long int).name()) == 0) { delete static_cast<long long int*>(dataObject->data); }
            else if (strcmp(type, typeid(unsigned long long int).name()) == 0) { delete static_cast<unsigned long long int*>(dataObject->data); }
            else if (strcmp(type, typeid(size_t).name()) == 0) { delete static_cast<size_t*>(dataObject->data); }
            else if (strcmp(type, typeid(float).name()) == 0) { delete static_cast<float*>(dataObject->data); }
            else if (strcmp(type, typeid(double).name()) == 0) { delete static_cast<double*>(dataObject->data); }
            else if (strcmp(type, typeid(long double).name()) == 0) { delete static_cast<long double*>(dataObject->data); }
            else if (strcmp(type, typeid(bool).name()) == 0) { delete static_cast<bool*>(dataObject->data); }
            else if (strcmp(type, typeid(char).name()) == 0) { delete static_cast<char*>(dataObject->data); }
            else if (strcmp(type, typeid(aea::sentence).name()) == 0) { delete static_cast<aea::sentence*>(dataObject->data); }

            dataObject->data = nullptr;
            dataObject->dataType = typeid(void);
            dataObject->isTrivial = false;
        }
    }


    template<typename T>
    void var::reset()
    {
        if (dataObject->data != nullptr)
        {
            delete static_cast<T*>(dataObject->data);

            dataObject->data = nullptr;
            dataObject->dataType = typeid(void);
            dataObject->isTrivial = false;
        }
    }


    template<typename T> 
    var::operator T()
    {
        if (dataObject->data != nullptr) { return *static_cast<T*>(dataObject->data); }
        return T();
    }


    template<typename T> 
    T& var_cast(var& obj)
    {
        return *static_cast<T*>(obj.dataObject->data);
    }

    template<typename T, typename...Args> 
    var make_var(Args&&...args)
    {
        var newObject = T(args...);
        return newObject;
    }


    const char* var::type() const
    {
        if (dataObject->data == nullptr) { return "\0"; }
        return dataObject->dataType.name();
    }


    bool var::isTrivial() const
    {
        return dataObject->isTrivial;
    }


    bool var::isData() const
    {
        return (dataObject->data != nullptr);
    }
}



#endif  // VAR_HPP