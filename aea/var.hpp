#ifndef VAR_HPP
#define VAR_HPP
#include "Functions.hpp"
#include "sentence.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>
#include <string.h>
#include <typeindex>
#include <typeinfo>
#include <math.h>
#include <cmath>
#include <memory>
#include <utility>
#include <initializer_list>
#include <vector>



namespace aea
{
    class var
    {
        public:
            var() = default;
            template<typename T> var(const T& obj);
            var(const var& obj);
            var(var&& obj);
            var(const std::string& obj);
            ~var();


            template<typename T> var& operator=(const T& obj);
            var& operator=(const var& obj);
            var& operator=(var&& obj);
            var& operator=(const std::string& obj);


            template<typename T> var& operator+=(const T& it);
            var& operator+=(const var& it);
            var& operator+=(const std::string& it);
            friend var operator+(const var& it, const var& it2);

            template<typename T> var& operator-=(const T& it);
            var& operator-=(const var& it);
            friend var operator-(const var& it, const var& it2);

            template<typename T> var& operator*=(const T& it);
            var& operator*=(const var& it);
            friend var operator*(const var& it, const var& it2);

            template<typename T> var& operator/=(const T& it);
            var& operator/=(const var& it);
            friend var operator/(const var& it, const var& it2);

            template<typename T> var& operator%=(const T& it);
            var& operator%=(const var& it);
            friend var operator%(const var& it, const var& it2);


            bool operator==(const var& it);
            bool operator!=(const var& it);
            bool operator>(const var& it);
            bool operator<(const var& it);
            bool operator>=(const var& it);
            bool operator<=(const var& it);

            template<typename T> operator T();

            template<typename T> void makeArray(const size_t& sz, const std::initializer_list<T>& list = {});
            template<typename T> void makeArray(const size_t& sz, const T* ptr);
            template<typename T> void makeArray(const std::vector<T>& it);
            void deleteArray();
            template<typename T> void deleteArray(); // Deallocation of pointed memory by parameter 'item->data' if 'makeArray()' method was called
            void resizeArray(const size_t& sz);
            template<typename T> void resizeArray(const size_t& sz);
            template<typename T> void assignArray(const std::initializer_list<size_t>& positions, const std::initializer_list<T>& list);
            template<typename T> void assignArray(const size_t& sz, const T* ptr);


            template<typename T> var& ref(T* it, const size_t& sz = 1);
            bool IsReffering() const;
            void unref();
            void dealloc(const std::string& mode = "delete");
            template<typename T> void dealloc(const std::string& mode = "delete"); // Deallocation of refered memory by parameter 'item->data' if 'ref()' method was called -> If second parameter was passed, will deallocate an array, else if only single item->data object
            void clear();
            template<typename T> void clear(); // Set all parametres of 'var' object on implicit values as a implicit constructor and deallocation of memory which is pointed by 'item->data' parameter if 'ref()' was not used and 'makeArray()' aswell
            void null();                                       // Set all parametres of 'var' object on implicit values as a implicit constructor


            friend std::ostream& operator<<(std::ostream& os, const var& obj);
            template<typename T> T& value(const size_t& num = 0) const;
            template<typename T = void> T* values() const;
            void getvar(const char& to = '\n');


            std::string type() const;
            size_t arraySize() const;
            template<typename T> size_t arraySize() const;
            bool IsArray() const;
            bool IsTrivial() const;
            size_t memsize() const;


        private:
            struct Data
            {
                void* data = nullptr;
                std::string* dataType = nullptr;
                bool isRef = false;
                bool isArray = false;
                std::uint64_t byte_size = 0;
            };

            Data* item = nullptr;


            class dataType_Spec
            {
                public:
                    dataType_Spec()
                    {
                        triv = new std::string("_triv");
                        nontriv = new std::string("_nontriv");
                    }

                    ~dataType_Spec()
                    {
                        delete triv;
                        delete nontriv;
                    }
                    

                    std::string* triv = nullptr;
                    std::string* nontriv = nullptr;
            };


            enum MathOperators
            {
                plus = '+',
                minus = '-',
                times = '*',
                devide = '/',
                modulo = '%'
            };


            void is_item_null();
            void is_item_dataType_null();
            friend bool operator==(const std::string& s, const std::string& s2);
            std::string strMinus(const std::string& it, const std::string& sub) const;
            bool contains(const std::string& it, const std::string& it2) const;
            template<typename T> void getDataType(T it, const bool& mensiTyp, const enum MathOperators& op); //Zjisti jakeho datoveho typu je hodnota, na kterou ukazuje ukazatel "item->data" a zjisti zda je tato hodnota mensiho, vetsiho nebo je roven datovemu typu hodnoty, se kterou chceme provest nejakou matematickou operaci
            void getDataTypeForVar(var it, const bool& mensiTyp, const enum MathOperators op);
            template<typename T, typename T2> void doOperation(T& hl, T2& vedl, const enum MathOperators& op); //Provadi matematicke operace
            template<typename T> void doOperationVar(T& hl, var& vedl, const size_t& thisSize, const size_t& vedlSize, const enum MathOperators op);
            template<typename T> void operatorModulo_func(T& temp, const var& it);
            template<typename T> void exception_func_for_operator_plus(const T& it);
            template<typename T> void exception_func_for_operators(const T& it, const enum MathOperators op);
            void exception_func_for_operators_var(const var& it, const enum MathOperators op);
            void exception_func_for_modulo(const var& it);
    };


    template<typename T>
    var::var(const T& obj)
    {
        item = new Data;
        item->dataType = new std::string;


        if (std::is_trivial<T>::value == true)
        {
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &obj, sizeof(T));

            *item->dataType = (typeid(T).name() + *dataType_Spec().triv);
        }

        else if (std::is_trivial<T>::value == false)
        {
            T* temp = new T;
            *temp = obj;
            item->data = temp;

            *item->dataType = (typeid(T).name() + *dataType_Spec().nontriv);
        }

        item->byte_size = sizeof(T);
    }


    var::var(const var& obj)
    {
        if (obj.item != nullptr && obj.item->dataType->empty() != true)
        {
            item = new Data;
            item->dataType = new std::string;


            if (obj.item->isRef == false && obj.item->isArray == false)
            {
                if (strMinus(*obj.item->dataType, *dataType_Spec().triv) == typeid(char*).name())
                {
                    const char* temp = static_cast<char*>(obj.item->data);
                    item->byte_size = obj.item->byte_size;

                    item->data = (void*) malloc(item->byte_size);
                    memcpy(item->data, obj.item->data, item->byte_size);

                    *item->dataType = *obj.item->dataType;
                    item->isRef = false;
                }

                else if (strMinus(*obj.item->dataType, *dataType_Spec().triv) != typeid(char*).name() && contains(*item->dataType, *dataType_Spec().nontriv) == false)
                {
                    item->byte_size = obj.memsize();

                    item->data = (void*) malloc(item->byte_size);
                    memcpy(item->data, obj.item->data, item->byte_size);

                    *item->dataType = *obj.item->dataType;
                    item->isRef = false;
                }
            }

            else if (obj.item->isRef == true)
            {
                item->data = obj.item->data;
                *item->dataType = *obj.item->dataType;
                item->isRef = true;
                item->byte_size = obj.item->byte_size;
                item->isArray = obj.item->isArray;
            }

            else if (obj.item->isRef == false && obj.item->isArray == true)
            {
                std::unique_ptr<std::string> minusedString(new std::string(strMinus(*obj.item->dataType, *dataType_Spec().triv)));

                if (*minusedString == typeid(var).name())
                {
                    var* temp = new var[obj.arraySize<var>()];
                    arrcpy(temp, static_cast<var*>(obj.item->data), obj.arraySize<var>());
                    item->data = temp;
                }

                else if (*minusedString == typeid(int).name())
                {
                    int* temp = new int[obj.arraySize<int>()];
                    memcpy(temp, static_cast<int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(unsigned int).name())
                {
                    unsigned int* temp = new unsigned int[obj.arraySize<unsigned int>()];
                    memcpy(temp, static_cast<unsigned int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(long int).name())
                {
                    long int* temp = new long int[obj.arraySize<long int>()];
                    memcpy(temp, static_cast<long int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(unsigned long int).name())
                {
                    unsigned long int* temp = new unsigned long int[obj.arraySize<unsigned long int>()];
                    memcpy(temp, static_cast<unsigned long int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(long long int).name())
                {
                    long long int* temp = new long long int[obj.arraySize<long long int>()];
                    memcpy(temp, static_cast<long long int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(unsigned long long int).name())
                {
                    unsigned long long int* temp = new unsigned long long int[obj.arraySize<unsigned long long int>()];
                    memcpy(temp, static_cast<unsigned long long int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(char*).name())
                {
                    char* temp = new char[obj.arraySize<char>()];
                    memcpy(temp, static_cast<char*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(size_t).name())
                {
                    size_t* temp = new size_t[obj.arraySize<size_t>()];
                    memcpy(temp, static_cast<size_t*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(short int).name())
                {
                    short int* temp = new short int[obj.arraySize<short int>()];
                    memcpy(temp, static_cast<short int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(unsigned short int).name())
                {
                    unsigned short int* temp = new unsigned short int[obj.arraySize<unsigned short int>()];
                    memcpy(temp, static_cast<unsigned short int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(float).name())
                {
                    float* temp = new float[obj.arraySize<float>()];
                    memcpy(temp, static_cast<float*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(double).name())
                {
                    double* temp = new double[obj.arraySize<double>()];
                    memcpy(temp, static_cast<double*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(long double).name())
                {
                    long double* temp = new long double[obj.arraySize<long double>()];
                    memcpy(temp, static_cast<long double*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                item->byte_size = obj.item->byte_size;
                *item->dataType = *obj.item->dataType;
                item->isArray = true;
            }
        }
    }


    var::var(var&& obj)
    {
        item = obj.item;
        obj.item = nullptr;
    }


    var::var(const std::string& obj)
    {
        item = new Data;
        item->dataType = new std::string;
        item->byte_size = obj.size() + 1;

        item->data = (void*) malloc(item->byte_size);
        arrcpy(static_cast<char*>(item->data), obj.data(), obj.size());
        (static_cast<char*>(item->data))[item->byte_size - 1] = '\0';

        *item->dataType = (typeid(char*).name() + *dataType_Spec().triv);
    }


    var::~var()
    {
        if (item != nullptr)
        {
            if (item->isRef == false && item->isArray == false)
            {
                if (item->data != nullptr && contains(*item->dataType, *dataType_Spec().triv) == true) { free((void*) item->data); }
            }

            else if (item->isRef == false && item->isArray == true)
            {
                if (item->data != nullptr)
                {
                    std::unique_ptr<std::string> minusedString (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

                    if (*minusedString == typeid(var).name())
                    {
                        if (item->byte_size == sizeof(var)) { delete (static_cast<var*>(item->data)); }
                        else if (item->byte_size > sizeof(var)) { delete [] (static_cast<var*>(item->data)); }
                    }

                    else if (*minusedString == typeid(int).name())
                    {
                        if (item->byte_size == sizeof(int)) { delete (static_cast<int*>(item->data)); }
                        else if (item->byte_size > sizeof(int)) { delete [] (static_cast<int*>(item->data)); }
                    }

                    else if (*minusedString == typeid(unsigned int).name())
                    {
                        if (item->byte_size == sizeof(unsigned int)) { delete (static_cast<unsigned int*>(item->data)); }
                        else if (item->byte_size > sizeof(unsigned int)) { delete [] (static_cast<unsigned int*>(item->data)); }
                    }

                    else if (*minusedString == typeid(long int).name())
                    {
                        if (item->byte_size == sizeof(long int)) { delete (static_cast<long int*>(item->data)); }
                        else if (item->byte_size > sizeof(long int)) { delete [] (static_cast<long int*>(item->data)); }
                    }

                    else if (*minusedString == typeid(unsigned long int).name())
                    {
                        if (item->byte_size == sizeof(unsigned long int)) { delete (static_cast<unsigned long int*>(item->data)); }
                        else if (item->byte_size > sizeof(unsigned long int)) { delete [] (static_cast<unsigned long int*>(item->data)); }
                    }

                    else if (*minusedString == typeid(long long int).name())
                    {
                        if (item->byte_size == sizeof(long long int)) { delete (static_cast<long long int*>(item->data)); }
                        else if (item->byte_size > sizeof(long long int)) { delete [] (static_cast<long long int*>(item->data)); }
                    }

                    else if (*minusedString == typeid(unsigned long long int).name())
                    {
                        if (item->byte_size == sizeof(unsigned long long int)) { delete (static_cast<unsigned long long int*>(item->data)); }
                        else if (item->byte_size > sizeof(unsigned long long int)) { delete [] (static_cast<unsigned long long int*>(item->data)); }
                    }

                    else if (*minusedString == typeid(char*).name())
                    {
                        if (item->byte_size == sizeof(char)) { delete (static_cast<char*>(item->data)); }
                        else if (item->byte_size > sizeof(char)) { delete [] (static_cast<char*>(item->data)); }
                    }

                    else if (*minusedString == typeid(size_t).name())
                    {
                        if (item->byte_size == sizeof(size_t)) { delete (static_cast<size_t*>(item->data)); }
                        else if (item->byte_size > sizeof(size_t)) { delete [] (static_cast<size_t*>(item->data)); }
                    }

                    else if (*minusedString == typeid(short int).name())
                    {
                        if (item->byte_size == sizeof(short int)) { delete (static_cast<short int*>(item->data)); }
                        else if (item->byte_size > sizeof(short int)) { delete [] (static_cast<short int*>(item->data)); }
                    }

                    else if (*minusedString == typeid(unsigned short int).name())
                    {
                        if (item->byte_size == sizeof(unsigned short int)) { delete (static_cast<unsigned short int*>(item->data)); }
                        else if (item->byte_size > sizeof(unsigned short int)) { delete [] (static_cast<unsigned short int*>(item->data)); }
                    }

                    else if (*minusedString == typeid(float).name())
                    {
                        if (item->byte_size == sizeof(float)) { delete (static_cast<float*>(item->data)); }
                        else if (item->byte_size > sizeof(float)) { delete [] (static_cast<float*>(item->data)); }
                    }

                    else if (*minusedString == typeid(double).name())
                    {
                        if (item->byte_size == sizeof(double)) { delete (static_cast<double*>(item->data)); }
                        else if (item->byte_size > sizeof(double)) { delete [] (static_cast<double*>(item->data)); }
                    }

                    else if (*minusedString == typeid(long double).name())
                    {
                        if (item->byte_size == sizeof(long double)) { delete (static_cast<long double*>(item->data)); }
                        else if (item->byte_size > sizeof(long double)) { delete [] (static_cast<long double*>(item->data)); }
                    }
                }
            }

            if (item->dataType != nullptr) { delete item->dataType; }
            delete item;
        }
    }


    template<typename T>
    var& var::operator=(const T& obj)
    {
        is_item_null();
        is_item_dataType_null();


        if (item->isRef == false && item->isArray == false)
        {
            if (item->data != nullptr && contains(*item->dataType, *dataType_Spec().nontriv) == false)
            {
                if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(var).name()) {  delete (static_cast<var*>(item->data));  }
                else { free((void*) item->data); }

                item->data = nullptr;
            }

            else if (item->data != nullptr && strMinus(*item->dataType, *dataType_Spec().nontriv) == typeid(T).name())
            {
                delete (static_cast<T*>(item->data));
                item->data = nullptr;
            }


            if (std::is_trivial<T>::value == true)
            {
                item->data = (void*) malloc(sizeof(T));
                memcpy(item->data, &obj, sizeof(T));
                *item->dataType = (typeid(T).name() + *dataType_Spec().triv);
            }

            else if (std::is_trivial<T>::value == false)
            {
                T* temp = new T;
                *temp = obj;
                item->data = temp;
                *item->dataType = (typeid(T).name() + *dataType_Spec().nontriv);
            }

            item->byte_size = sizeof(T);
        }

        else if (item->isRef == true)
        {
            if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(T).name() || strMinus(*item->dataType, *dataType_Spec().nontriv) == typeid(T).name())
            {
                *static_cast<T*>(item->data) = obj;
            }
        }

        else if (item->isRef == false && item->isArray == true)
        {
            if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(T).name() || strMinus(*item->dataType, *dataType_Spec().nontriv) == typeid(T).name())
            {
                *static_cast<T*>(item->data) = obj;
            }
        }

        return *this;
    }


    var& var::operator=(const var& obj)
    {
        if (obj.item != nullptr && obj.item->dataType->empty() != true)
        {
            is_item_null();
            is_item_dataType_null();


            if (obj.item->isRef == false && obj.item->isArray == false)
            {
                if (strMinus(*obj.item->dataType, *dataType_Spec().triv) == typeid(char*).name() && contains(*item->dataType, *dataType_Spec().nontriv) == false)
                {
                    if (item->data != nullptr && item->isRef == false)
                    {
                        if (item->isArray == true)
                        {
                            deleteArray();
                        }

                        else
                        {
                            free((void*)item->data);
                        }

                        item->data = nullptr;
                    }


                    const char* temp = static_cast<char*>(obj.item->data);
                    item->byte_size = obj.item->byte_size;

                    item->data = (void*)malloc(item->byte_size);
                    memcpy(item->data, obj.item->data, item->byte_size);

                    *item->dataType = *obj.item->dataType;
                    item->isRef = false;
                    item->isArray = false;
                }

                else if (strMinus(*obj.item->dataType, *dataType_Spec().triv) != typeid(char*).name() && contains(*item->dataType, *dataType_Spec().nontriv) == false)
                {
                    if (item->data != nullptr && item->isRef == false)
                    {
                        if (item->isArray == true)
                        {
                            deleteArray();
                        }

                        else if (item->isArray == false)
                        {
                            free((void*)item->data);
                        }

                        item->data = nullptr;
                    }

                    item->byte_size = obj.item->byte_size;

                    item->data = (void*)malloc(item->byte_size);
                    memcpy(item->data, obj.item->data, item->byte_size);

                    *item->dataType = *obj.item->dataType;
                    item->isRef = false;
                    item->isArray = false;
                }
            }

            else if (obj.item->isRef == true)
            {
                if (*item->dataType == *obj.item->dataType || item->data == nullptr)
                {
                    if (item->data != nullptr && item->isRef == false)
                    {
                        if (item->isArray == true && contains(*item->dataType, *dataType_Spec().nontriv) == false)
                        {
                            deleteArray();
                        }

                        else if (item->isArray == false && contains(*item->dataType, *dataType_Spec().nontriv) == false)
                        {
                            free((void*)item->data);
                        }

                        item->data = nullptr;
                    }

                    item->data = obj.item->data;
                    *item->dataType = *obj.item->dataType;
                    item->isRef = true;
                    item->byte_size = obj.item->byte_size;
                    item->isArray = obj.item->isArray;
                }
            }


            else if (obj.item->isRef == false && obj.item->isArray == true)
            {
                if (item->data != nullptr && item->isRef == false)
                {
                    if (item->isArray == true && contains(*item->dataType, *dataType_Spec().nontriv) == false)
                    {
                        deleteArray();
                    }

                    else if (item->isArray == false && contains(*item->dataType, *dataType_Spec().nontriv) == false)
                    {
                        free((void*)item->data);
                    }

                    item->data = nullptr;
                }


                std::unique_ptr<std::string> minusedString(new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

                if (*minusedString == typeid(var).name())
                {
                    var* temp = new var[obj.arraySize<var>()];
                    arrcpy(temp, static_cast<var*>(obj.item->data), obj.arraySize<var>());
                    item->data = temp;
                }

                else if (*minusedString == typeid(int).name())
                {
                    int* temp = new int[obj.arraySize<int>()];
                    memcpy(temp, static_cast<int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(unsigned int).name())
                {
                    unsigned int* temp = new unsigned int[obj.arraySize<unsigned int>()];
                    memcpy(temp, static_cast<unsigned int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(long int).name())
                {
                    long int* temp = new long int[obj.arraySize<long int>()];
                    memcpy(temp, static_cast<long int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(unsigned long int).name())
                {
                    unsigned long int* temp = new unsigned long int[obj.arraySize<unsigned long int>()];
                    memcpy(temp, static_cast<unsigned long int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(long long int).name())
                {
                    long long int* temp = new long long int[obj.arraySize<long long int>()];
                    memcpy(temp, static_cast<long long int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(unsigned long long int).name())
                {
                    unsigned long long int* temp = new unsigned long long int[obj.arraySize<unsigned long long int>()];
                    memcpy(temp, static_cast<unsigned long long int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(char*).name())
                {
                    char* temp = new char[obj.arraySize<char>()];
                    memcpy(temp, static_cast<char*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(size_t).name())
                {
                    size_t* temp = new size_t[obj.arraySize<size_t>()];
                    memcpy(temp, static_cast<size_t*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(short int).name())
                {
                    short int* temp = new short int[obj.arraySize<short int>()];
                    memcpy(temp, static_cast<short int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(unsigned short int).name())
                {
                    unsigned short int* temp = new unsigned short int[obj.arraySize<unsigned short int>()];
                    memcpy(temp, static_cast<unsigned short int*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(float).name())
                {
                    float* temp = new float[obj.arraySize<float>()];
                    memcpy(temp, static_cast<float*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(double).name())
                {
                    double* temp = new double[obj.arraySize<double>()];
                    memcpy(temp, static_cast<double*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                else if (*minusedString == typeid(long double).name())
                {
                    long double* temp = new long double[obj.arraySize<long double>()];
                    memcpy(temp, static_cast<long double*>(obj.item->data), obj.memsize());
                    item->data = temp;
                }

                item->byte_size = obj.item->byte_size;
                *item->dataType = *obj.item->dataType;
                item->isRef = false;
                item->isArray = true;
            }
        }

        else
        {
            if (item != nullptr && item->data != nullptr && item->isRef == false && item->dataType->empty() != true)
            {
                if (item->isArray == true)
                {
                    deleteArray();
                }

                else
                {
                    free((void*) item->data);
                }

                delete item->dataType; 
                delete item;
                item = nullptr;
            }
        }

        return *this;
    }


    var& var::operator=(var&& obj)
    {
        if (item != nullptr)
        {
            clear();
        }
        
        
        item = obj.item;
        obj.item = nullptr;
        
        return *this;
    }


    var& var::operator=(const std::string& obj)
    {
        is_item_null();
        is_item_dataType_null();


        if (item->data != nullptr && item->isRef == false)
        {
            if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(var).name())
            {
                if (item->isArray == false) { delete (static_cast<var*>(item->data)); }
                else if (item->isArray == true) { delete (static_cast<var*>(item->data)); }

                item->data = nullptr;
            }

            else if (item->isArray == true && contains(*item->dataType, *dataType_Spec().nontriv) == false)
            {
                deleteArray();
            }

            else if (item->isArray == false && contains(*item->dataType, *dataType_Spec().nontriv) == false)
            {
                free((void*) item->data);
                item->data = nullptr;
            }

            item->byte_size = obj.size() + 1;

            item->data = (void*) malloc(item->byte_size);
            memcpy(item->data, obj.data(), obj.size());
            (static_cast<char*>(item->data))[obj.size()] = '\0';

            *item->dataType = (typeid(char*).name() + *dataType_Spec().triv);
            item->isArray = false;
        }

        else if (item->isRef == true)
        {
            if (obj.size() == (item->byte_size - 1))
            {
                memcpy(item->data, obj.data(), obj.size());
                (static_cast<char*>(item->data))[obj.size()] = '\0';
            }
        }

        return *this;
    }


    template<typename T>
    void var::makeArray(const size_t& sz, const std::initializer_list<T>& list)
    {
        is_item_null();
        is_item_dataType_null();


        if (item->data != nullptr && item->isArray == true && contains(*item->dataType, *dataType_Spec().nontriv) == true)
        {
            throw std::runtime_error("bug: Allocating new memory causes a memory leak");
        }

        else if (sz == 0)
        {
            throw std::length_error("bug: Can not make a data array of size '0'");
        }



        if (item->data != nullptr && item->isRef == false)
        {
            if (item->isArray == true) { deleteArray(); }
            else if (item->isArray == false) { free((void*) item->data); }

            item->data = nullptr;
        }
        

        T* temp = new T[sz];

        if (list.size() > 0)
        {
            for (size_t i = 0; i < list.size(); i++)
            {
                temp[i] = *(list.begin() + i);
            }
        }

        item->data = temp;
        
        if (std::is_trivial<T>::value == true || typeid(T).name() == typeid(var).name()) { *item->dataType = (typeid(T).name() + *dataType_Spec().triv); }
        else if (std::is_trivial<T>::value == false && typeid(T).name() != typeid(var).name()) { *item->dataType = (typeid(T).name() + *dataType_Spec().nontriv); }

        item->byte_size = sz * sizeof(T);
        item->isArray = true;
    }


    template<typename T>
    void var::makeArray(const size_t& sz, const T* ptr)
    {
        is_item_null();
        is_item_dataType_null();


        if (item->data != nullptr && item->isArray == true && contains(*item->dataType, *dataType_Spec().nontriv) == true)
        {
            throw std::runtime_error("bug: Allocating new memory causes a memory leak");
        }

        else if (sz == 0 || ptr == nullptr)
        {
            throw std::length_error("bug: Can not make a data array of size '0'");
        }


        if (item->data != nullptr && item->isRef == false)
        {
            if (item->isArray == true) { deleteArray(); }
            else if (item->isArray == false) { free((void*) item->data); }

            item->data = nullptr;
        }

        T* temp = new T[sz];

        for (size_t i = 0; i < sz; i++)
        {
            temp[i] = ptr[i];
            i += 1;
        }

        item->data = temp;

        if (std::is_trivial<T>::value == true || typeid(T).name() == typeid(var).name()) { *item->dataType = (typeid(T).name() + *dataType_Spec().triv); }
        else if (std::is_trivial<T>::value == false) { *item->dataType = (typeid(T).name() + *dataType_Spec().nontriv); }

        item->byte_size = sz * sizeof(T);
        item->isArray = true;
    }


    template<typename T>
    void var::makeArray(const std::vector<T>& it)
    {
        is_item_null();
        is_item_dataType_null();

        if (item->data != nullptr && item->isArray == true)
        {
            throw std::runtime_error("bug: Allocating new memory causes a memory leak");
        }

        else if (it.size() == 0)
        {
            throw std::length_error("bug: Can not make a data array of size '0'");
        }


        T* temp = new T[it.size()];

        for (size_t i = 0; i < it.size(); i++)
        {
            temp[i] = it[i];
        }

        item->data = temp;

        if (std::is_trivial<T>::value == true || typeid(T).name() == typeid(var).name()) { *item->dataType = (typeid(T).name() + *dataType_Spec().triv); }
        else if (std::is_trivial<T>::value == false) { *item->dataType = (typeid(T).name() + *dataType_Spec().nontriv); }

        item->isArray = true;
        item->byte_size = it.size() * sizeof(T);
    }


    void var::deleteArray()
    {
        if (item == nullptr || item->data == nullptr || item->isRef == true || item->isArray == false || item->dataType == nullptr || item->dataType->empty() == true)
        {
            throw std::runtime_error("error: Can not deallocate any memory");
        }


        if (item->dataType->empty() != true)
        {
            std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));


            if (*minusedStr == typeid(var).name())
            {
                if (item->byte_size == sizeof(var)) { delete (static_cast<var*>(item->data)); }
                else if (item->byte_size > sizeof(var)) { delete [] (static_cast<var*>(item->data)); }
            }

            else if (*minusedStr == typeid(int).name())
            {
                if (item->byte_size == sizeof(int)) { delete (static_cast<int*>(item->data)); }
                else if (item->byte_size > sizeof(int)) { delete [] (static_cast<int*>(item->data)); }
            }

            else if (*minusedStr == typeid(unsigned int).name())
            {
                if (item->byte_size == sizeof(unsigned int)) { delete (static_cast<unsigned int*>(item->data)); }
                else if (item->byte_size > sizeof(unsigned int)) { delete [] (static_cast<unsigned int*>(item->data)); }
            }

            else if (*minusedStr == typeid(long int).name())
            {
                if (item->byte_size == sizeof(long int)) { delete (static_cast<long int*>(item->data)); }
                else if (item->byte_size > sizeof(long int)) { delete [] (static_cast<long int*>(item->data)); }
            }

            else if (*minusedStr == typeid(unsigned long int).name())
            {
                if (item->byte_size == sizeof(unsigned long int)) { delete (static_cast<unsigned long int*>(item->data)); }
                else if (item->byte_size > sizeof(unsigned long int)) { delete [] (static_cast<unsigned long int*>(item->data)); }
            }

            else if (*minusedStr == typeid(long long int).name())
            {
                if (item->byte_size == sizeof(long long int)) { delete (static_cast<long long int*>(item->data)); }
                else if (item->byte_size > sizeof(long long int)) { delete [] (static_cast<long long int*>(item->data)); }
            }

            else if (*minusedStr == typeid(unsigned long long int).name())
            {
                if (item->byte_size == sizeof(unsigned long long int)) { delete (static_cast<unsigned long long int*>(item->data)); }
                else if (item->byte_size > sizeof(unsigned long long int)) { delete [] (static_cast<unsigned long long int*>(item->data)); }
            }

            else if (*minusedStr == typeid(char*).name())
            {
                if (item->byte_size == sizeof(char)) { delete (static_cast<char*>(item->data)); }
                else if (item->byte_size > sizeof(char)) { delete [] (static_cast<char*>(item->data)); }
            }

            else if (*minusedStr == typeid(size_t).name())
            {
                if (item->byte_size == sizeof(size_t)) { delete (static_cast<size_t*>(item->data)); }
                else if (item->byte_size > sizeof(size_t)) { delete [] (static_cast<size_t*>(item->data)); }
            }

            else if (*minusedStr == typeid(short int).name())
            {
                if (item->byte_size == sizeof(short int)) { delete (static_cast<short int*>(item->data)); }
                else if (item->byte_size > sizeof(short int)) { delete [] (static_cast<short int*>(item->data)); }
            }

            else if (*minusedStr == typeid(unsigned short int).name())
            {
                if (item->byte_size == sizeof(unsigned short int)) { delete (static_cast<unsigned short int*>(item->data)); }
                else if (item->byte_size > sizeof(unsigned short int)) { delete [] (static_cast<unsigned short int*>(item->data)); }
            }

            else if (*minusedStr == typeid(float).name())
            {
                if (item->byte_size == sizeof(float)) { delete (static_cast<float*>(item->data)); }
                else if (item->byte_size > sizeof(float)) { delete [] (static_cast<float*>(item->data)); }
            }

            else if (*minusedStr == typeid(double).name())
            {
                if (item->byte_size == sizeof(double)) { delete (static_cast<double*>(item->data)); }
                else if (item->byte_size > sizeof(double)) { delete [] (static_cast<double*>(item->data)); }
            }

            else if  (*minusedStr == typeid(long double).name())
            {
                if (item->byte_size == sizeof(long double)) { delete (static_cast<long double*>(item->data)); }
                else if (item->byte_size > sizeof(long double)) { delete [] (static_cast<long double*>(item->data)); }
            }
        }

        item->data = nullptr;
        item->byte_size = 0;
    }


    template<typename T>
    void var::deleteArray()
    {
        if (item == nullptr || item->data == nullptr || item->isRef == true || item->isArray == false || item->dataType == nullptr || item->dataType->empty() == true)
        {
            throw std::runtime_error("error: Can not deallocate any memory");
        }


        if (item->byte_size == sizeof(T) && typeid(T).name() != typeid(char).name()) { delete (static_cast<T*>(item->data)); }
        else if (item->byte_size > sizeof(T) && typeid(T).name() != typeid(char).name()) { delete [] (static_cast<T*>(item->data)); }

        item->data = nullptr;
        item->byte_size = 0;
    }


    size_t var::arraySize() const
    {
        size_t retValue = 0;


        if (item == nullptr || item->dataType->empty() == true || item->isArray == false) { retValue = 0; }

        else if (contains(*item->dataType, *dataType_Spec().nontriv) == true) { retValue = item->byte_size; }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(var).name()) { retValue = (item->byte_size / sizeof(var)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(int).name()) { retValue = (item->byte_size / sizeof(int)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(unsigned int).name()) { retValue = (item->byte_size / sizeof(unsigned int)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(long int).name()) { retValue = (item->byte_size / sizeof(long int)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(unsigned long int).name()) { retValue = (item->byte_size / sizeof(unsigned long int)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(long long int).name()) { retValue = (item->byte_size / sizeof(long long int)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(unsigned long long int).name()) { retValue = (item->byte_size / sizeof(unsigned long long int)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(char*).name()) { retValue = item->byte_size; }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(size_t).name()) { retValue = (item->byte_size / sizeof(size_t)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(short int).name()) { retValue = (item->byte_size / sizeof(short int)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(unsigned short int).name()) { retValue = sizeof(unsigned short int); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(float).name()) { retValue = (item->byte_size / sizeof(float)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(double).name()) { retValue = (item->byte_size / sizeof(double)); }

        else if (strMinus(*item->dataType, *dataType_Spec().triv) == typeid(long double).name()) { retValue = (item->byte_size / sizeof(long double)); }


        return retValue;
    }


    void var::resizeArray(const size_t& sz)
    {
        if (item == nullptr || item->data == nullptr || item->isRef == true || item->isArray == false)
        {
            throw std::runtime_error("bug: Can not reallocate any memory");
        }

        else if (sz == 0)
        {
            throw std::length_error("bug: Can not resize a data array on a size '0'");
        }


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));


        if (*minusedStr == typeid(var).name())
        {
            var* temp = new var[sz];

            if ((sz * sizeof(var)) > item->byte_size) { arrcpy(temp, static_cast<var*>(item->data), (item->byte_size / sizeof(var))); }
            else { arrcpy(temp, static_cast<var*>(item->data), sz); }

            delete [] (static_cast<var*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(var) * sz;
        }

        else if (*minusedStr == typeid(int).name())
        {
            int* temp = new int[sz];

            if ((sz * sizeof(int)) > item->byte_size) { arrcpy(temp, static_cast<int*>(item->data), (item->byte_size / sizeof(int))); }
            else { arrcpy(temp, static_cast<int*>(item->data), sz); }

            delete [] (static_cast<int*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(int) * sz;
        }

        else if (*minusedStr == typeid(unsigned int).name())
        {
            unsigned int* temp = new unsigned int[sz];

            if ((sz * sizeof(unsigned int)) > item->byte_size) { arrcpy(temp, static_cast<unsigned int*>(item->data), (item->byte_size / sizeof(unsigned int))); }
            else { arrcpy(temp, static_cast<unsigned int*>(item->data), sz); }

            delete [] (static_cast<unsigned int*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(unsigned int) * sz;
        }

        else if (*minusedStr == typeid(long int).name())
        {
            long int* temp = new long int[sz];

            if ((sz * sizeof(long int)) > item->byte_size) { arrcpy(temp, static_cast<long int*>(item->data), (item->byte_size / sizeof(long int))); }
            else { arrcpy(temp, static_cast<long int*>(item->data), sz); }

            delete [] (static_cast<long int*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(long int) * sz;
        }

        else if (*minusedStr == typeid(unsigned long int).name())
        {
            unsigned long int* temp = new unsigned long int[sz];

            if ((sz * sizeof(unsigned long int)) > item->byte_size) { arrcpy(temp, static_cast<unsigned long int*>(item->data), (item->byte_size / sizeof(unsigned long int))); }
            else { arrcpy(temp, static_cast<unsigned long int*>(item->data), sz); }

            delete [] (static_cast<unsigned long int*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(unsigned long int) * sz;
        }

        else if (*minusedStr == typeid(long long int).name())
        {
            long long int* temp = new long long int[sz];

            if ((sz * sizeof(long long int)) > item->byte_size) { arrcpy(temp, static_cast<long long int*>(item->data), (item->byte_size / sizeof(long long int))); }
            else { arrcpy(temp, static_cast<long long int*>(item->data), sz); }

            delete [] (static_cast<long long int*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(long long int) * sz;
        }

        else if (*minusedStr == typeid(unsigned long long int).name())
        {
            unsigned long long int* temp = new unsigned long long int[sz];

            if ((sz * sizeof(unsigned long long int)) > item->byte_size) { arrcpy(temp, static_cast<unsigned long long int*>(item->data), (item->byte_size / sizeof(unsigned long long int))); }
            else { arrcpy(temp, static_cast<unsigned long long int*>(item->data), sz); }

            delete [] (static_cast<unsigned long long int*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(unsigned long long int) * sz;
        }

        else if (*minusedStr == typeid(char*).name())
        {
            char* temp = new char[sz];

            if ((sz * sizeof(char)) > item->byte_size) { arrcpy(temp, static_cast<char*>(item->data), (item->byte_size / sizeof(char))); }
            else { arrcpy(temp, static_cast<char*>(item->data), sz); }

            delete [] (static_cast<char*>(item->data));
            item->data = temp;

            item->byte_size = sz;
        }

        else if (*minusedStr == typeid(size_t).name())
        {
            size_t* temp = new size_t[sz];

            if ((sz * sizeof(size_t)) > item->byte_size) { arrcpy(temp, static_cast<size_t*>(item->data), (item->byte_size / sizeof(size_t))); }
            else { arrcpy(temp, static_cast<size_t*>(item->data), sz); }

            delete [] (static_cast<size_t*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(size_t) * sz;
        }

        else if (*minusedStr == typeid(short int).name())
        {
            short int* temp = new short int[sz];

            if ((sz * sizeof(short int)) > item->byte_size) { arrcpy(temp, static_cast<short int*>(item->data), (item->byte_size / sizeof(short int))); }
            else { arrcpy(temp, static_cast<short int*>(item->data), sz); }

            delete [] (static_cast<short int*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(short int) * sz;
        }

        else if (*minusedStr == typeid(unsigned short int).name())
        {
            unsigned short int* temp = new unsigned short int[sz];

            if ((sz * sizeof(unsigned short int)) > item->byte_size) { arrcpy(temp, static_cast<unsigned short int*>(item->data), (item->byte_size / sizeof(unsigned short int))); }
            else { arrcpy(temp, static_cast<unsigned short int*>(item->data), sz); }

            delete [] (static_cast<unsigned short int*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(unsigned short int) * sz;
        }

        else if (*minusedStr == typeid(float).name())
        {
            float* temp = new float[sz];

            if ((sz * sizeof(float)) > item->byte_size) { arrcpy(temp, static_cast<float*>(item->data), (item->byte_size / sizeof(float))); }
            else { arrcpy(temp, static_cast<float*>(item->data), sz); }

            delete [] (static_cast<float*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(float) * sz;
        }

        else if (*minusedStr == typeid(double).name())
        {
            double* temp = new double[sz];

            if ((sz * sizeof(double)) > item->byte_size) { arrcpy(temp, static_cast<double*>(item->data), (item->byte_size / sizeof(double))); }
            else { arrcpy(temp, static_cast<double*>(item->data), sz); }

            delete [] (static_cast<double*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(double) * sz;
        }

        else if (*minusedStr == typeid(long double).name())
        {
            long double* temp = new long double[sz];

            if ((sz * sizeof(long double)) > item->byte_size) { arrcpy(temp, static_cast<long double*>(item->data), (item->byte_size / sizeof(long double))); }
            else { arrcpy(temp, static_cast<long double*>(item->data), sz); }

            delete [] (static_cast<long double*>(item->data));
            item->data = temp;

            item->byte_size = sizeof(long double) * sz;
        }
    }


    template<typename T>
    void var::resizeArray(const size_t& sz)
    {
        is_item_null();
        is_item_dataType_null();

        if (item == nullptr || item->data == nullptr || item->isRef == true || item->isArray == false)
        {
            throw std::runtime_error("bug: Can not reallocate any memory");
        }

        else if (sz == 0)
        {
            throw std::length_error("bug: Can not resize a data array on a size '0'");
        }


        T* new_arr = new T[sz];

        if ((sz * sizeof(T)) > item->byte_size) { arrcpy(new_arr, static_cast<T*>(item->data), (item->byte_size / sizeof(T))); }
        else { arrcpy(new_arr, static_cast<T*>(item->data), sz); }

        deleteArray<T>();
        item->data = new_arr;
        item->byte_size = sz * sizeof(T);
    }


    template<typename T>
    void var::assignArray(const std::initializer_list<size_t>& positions, const std::initializer_list<T>& list)
    {
        is_item_null();
        is_item_dataType_null();

        if ((item->isRef == false && item->isArray == false) || item->dataType->empty() == true)
        {
            throw std::out_of_range("bug: No accessible memory");
        }


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));


        if (strMinus(*item->dataType, *dataType_Spec().nontriv) == typeid(T).name())
        {
            T* const temp = static_cast<T*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(var).name())
        {
            var* const temp = static_cast<var*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(int).name())
        {
            int* const temp = static_cast<int*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(unsigned int).name())
        {
            unsigned int* const temp = static_cast<unsigned int*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(long int).name())
        {
            long int* const temp = static_cast<long int*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(unsigned long int).name())
        {
            unsigned long int* const temp = static_cast<unsigned long int*>(item->data);

        for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(long long int).name())
        {
            long long int* const temp = static_cast<long long int*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(unsigned long long int).name())
        {
            unsigned long long int* const temp = static_cast<unsigned long long int*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(char*).name())
        {
            char* temp = static_cast<char*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(size_t).name())
        {
            size_t* temp = static_cast<size_t*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(short int).name())
        {
            short int* temp = static_cast<short int*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(unsigned short int).name())
        {
            unsigned short int* temp = static_cast<unsigned short int*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(float).name())
        {
            float* temp = static_cast<float*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(double).name())
        {
            double* temp = static_cast<double*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }

        else if (*minusedStr == typeid(long double).name())
        {
            long double* temp = static_cast<long double*>(item->data);

            for (size_t i = 0; i < list.size(); i++)
            {
                temp[*(positions.begin() + i)] = *(list.begin() + i);
            }
        }
    }


    template<typename T>
    void var::assignArray(const size_t& sz, const T* ptr)
    {
        is_item_null();
        is_item_dataType_null();

        if ((item->isRef == false && item->isArray == false) || item->dataType->empty() == true || sz == 0 || ptr == nullptr)
        {
            throw std::out_of_range("bug: No accessible memory");
        }

        else if (typeid(T).name() == typeid(char*).name() && strMinus(*item->dataType, *dataType_Spec().triv) != typeid(char*).name())
        {
            std::unique_ptr<std::string> errorMessage (new std::string());
            
            *errorMessage.get() = "error: Invalid conversion from '";
            *errorMessage.get() += type();
            *errorMessage.get() += "' to '";
            *errorMessage.get() +=  typeid(char*).name();
            *errorMessage.get() += "'";

            throw std::runtime_error(*errorMessage.get());
        }

        else if (typeid(T).name() != typeid(char*).name() && strMinus(*item->dataType, *dataType_Spec().triv) == typeid(char*).name())
        {
            std::unique_ptr<std::string> errorMessage (new std::string());
            
            *errorMessage.get() = "error: Invalid conversion from '";
            *errorMessage.get() += typeid(char*).name();
            *errorMessage.get() += "' to '";
            *errorMessage.get() +=  type();
            *errorMessage.get() += "'";

            throw std::runtime_error(*errorMessage.get());
        }


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));


        if (strMinus(*item->dataType, *dataType_Spec().nontriv) == typeid(T).name() && strcmp(typeid(T).name(), typeid(size_t).name()) != 0)
        {
            T* const temp = static_cast<T*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(var).name())
        {
            var* const temp = static_cast<var*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(int).name())
        {
            int* const temp = static_cast<int*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(unsigned int).name())
        {
            unsigned int* const temp = static_cast<unsigned int*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(long int).name())
        {
            long int* const temp = static_cast<long int*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(unsigned long int).name())
        {
            unsigned long int* const temp = static_cast<unsigned long int*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(long long int).name())
        {
            long long int* const temp = static_cast<long long int*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(unsigned long long int).name())
        {
            unsigned long long int* const temp = static_cast<unsigned long long int*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(char*).name())
        {
            char* const temp = static_cast<char*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(size_t).name())
        {
            size_t* const temp = static_cast<size_t*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(short int).name())
        {
            short int* const temp = static_cast<short int*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(unsigned short int).name())
        {
            unsigned short int* const temp = static_cast<unsigned short int*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(float).name())
        {
            float* const temp = static_cast<float*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(double).name())
        {
            double* const temp = static_cast<double*>(item->data);
            arrcpy(temp, ptr, sz);
        }

        else if (*minusedStr == typeid(long double).name())
        {
            long double* const temp = static_cast<long double*>(item->data);
            arrcpy(temp, ptr, sz);
        }
    }


    template<typename T>
    size_t var::arraySize() const
    {
        if (item == nullptr || item->dataType->empty() == true || item->isArray == false) { return 0; }
        else { return (item->byte_size / sizeof(T)); }
    }


    template<typename T>
    var& var::ref(T* it, const size_t& sz)
    {
        is_item_null();
        is_item_dataType_null();

        if (sz == 0)
        {
            throw std::length_error("bug: Can not refer on data array of size '0'");
        }


        if (item->isRef == false && item->isArray == false && contains(*item->dataType, *dataType_Spec().nontriv) == false && item->data != nullptr) { free((void*) item->data); }
        else if (item->isRef == false && item->isArray == true && contains(*item->dataType, *dataType_Spec().nontriv) == false) { deleteArray(); }
        item->data = nullptr;

        item->data = it;

        if (sz == 1) { item->byte_size = sizeof(T); }
        else if (sz > 1)
        {
            item->byte_size = sizeof(T) * sz;
            item->isArray = true;
        }

        if (std::is_trivial<T>::value == true) { *item->dataType = (typeid(T).name() + *dataType_Spec().triv); }
        else if (std::is_trivial<T>::value == false) { *item->dataType = (typeid(T).name() + *dataType_Spec().nontriv); }

        item->isRef = true;

        return *this;
    }


    void var::unref()
    {
        if (item != nullptr)
        {
            item->data = nullptr;
            item->isRef = false;
            item->isArray = false;
            item->byte_size = 0;

            if (item->dataType != nullptr)
            {
                delete item->dataType;
                item->dataType = nullptr;
            }
        }
    }


    void var::dealloc(const std::string& mode)
    {
        if (item == nullptr || item->dataType == nullptr || item->isRef == false || (mode != "free" && mode != "delete") || item->dataType->empty() == true)
        {
            throw std::runtime_error("error: Can not deallocate any memory");
        }


        if (mode == "free")
        {
            if (item->data != nullptr) { free((void*) item->data); }
        }

        else if (mode == "delete")
        {
            if (item->data != nullptr)
            {
                std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

                if (*minusedStr == typeid(var).name())
                {
                    var* temp = (static_cast<var*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(int).name())
                {
                    int* temp = (static_cast<int*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(unsigned int).name())
                {
                    unsigned int* temp = (static_cast<unsigned int*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(long int).name())
                {
                    long int* temp = (static_cast<long int*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(unsigned long int).name())
                {
                    unsigned long int* temp = (static_cast<unsigned long int*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(long long int).name())
                {
                    long long int* temp = (static_cast<long long int*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(unsigned long long int).name())
                {
                    unsigned long long int* temp = (static_cast<unsigned long long int*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(char*).name())
                {
                    char* temp = (static_cast<char*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(size_t).name())
                {
                    size_t* temp = (static_cast<size_t*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(short int).name())
                {
                    short int* temp = (static_cast<short int*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(unsigned short int).name())
                {
                    unsigned short int* temp = (static_cast<unsigned short int*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(float).name())
                {
                    float* temp = (static_cast<float*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(double).name())
                {
                    double* temp = (static_cast<double*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }

                else if (*minusedStr == typeid(long double).name())
                {
                    long double* temp = (static_cast<long double*>(item->data));

                    if (item->isArray == false) { delete temp; }
                    else if (item->isArray == true) { delete [] temp; }
                }
            }
        }
    }


    template<typename T>
    void var::dealloc(const std::string& mode)
    {
        if (item == nullptr || item->dataType == nullptr || item->isRef == false || (mode != "free" && mode != "delete") || item->dataType->empty() == true)
        {
            throw std::runtime_error("error: Can not deallocate any memory");
        }


        if (mode == "free" && item->data != nullptr) { free((void*) item->data); }

        else if (mode == "delete" && item->data != nullptr)
        {
            T* temp_ptr = (static_cast<T*>(item->data));

            if (item->isArray == false) { delete temp_ptr; }
            else if (item->isArray == true) { delete [] temp_ptr; }
        }
    }


    void var::clear()
    {
        if (item != nullptr)
        {
            if (item->data != nullptr && item->dataType->empty() != true)
            {
                if (item->isArray == true && item->isRef == false && contains(*item->dataType, *dataType_Spec().nontriv) == false) { deleteArray(); }  //array = true && Nontriv, ref = false
                else if (item->isArray == false && item->isRef == false && contains(*item->dataType, *dataType_Spec().nontriv) == false) { free((void*) item->data); }  //Nontriv, array, ref = false && typeid != var

                item->data = nullptr;
            }

            if (item->dataType != nullptr)
            {
                delete item->dataType;
            }

            delete item;
            item = nullptr;
        }
    }


    template<typename T>
    void var::clear()
    {
        if (item != nullptr)
        {
            if (item->data != nullptr && item->dataType->empty() != true)
            {
                if (item->isArray == true && item->isRef == false && contains(*item->dataType, *dataType_Spec().nontriv) == false) { deleteArray(); }  //array = true && Nontriv, ref = false
                else if (item->isArray == false && item->isRef == false && contains(*item->dataType, *dataType_Spec().nontriv) == true) { delete (static_cast<T*>(item->data)); }  //Nontriv = true && array, ref = false
                else if (item->isArray == false && item->isRef == false && contains(*item->dataType, *dataType_Spec().nontriv) == false) { free((void*)item->data); }  //Nontriv, array, ref = false && typeid != var

                item->data = nullptr;
            }

            if (item->dataType != nullptr)
            {
                delete item->dataType;
            }

            delete item;
            item = nullptr;
        }
    }


    void var::null()
    {
        if (item != nullptr)
        {
            item->data = nullptr;
            item->isRef = false;
            item->isArray = false;
            item->byte_size = 0;

            if (item->dataType != nullptr)
            {
                delete item->dataType;
                item->dataType = nullptr;
            }

            delete item;
            item = nullptr;
        }
    }


    template<typename T>
    var::operator T()
    {
        is_item_null();
        is_item_dataType_null();


        T retObj;
        if (item->dataType->empty() == false) { retObj = *(static_cast<T*>(item->data)); }

        return retObj;
    }


    bool var::operator==(const var& it)
    {
        is_item_null();
        is_item_dataType_null();


        if (contains(*item->dataType, *dataType_Spec().triv) == true)
        {
            std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

            if (*minusedStr == typeid(var).name())
            {
                var* const this_ptr = (static_cast<var*>(item->data));
                const var* const it_ptr = (static_cast<var*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<var>() != it.arraySize<var>()) { return false; }

                    for (size_t i = 0; i < arraySize<var>(); i++)
                    {
                        if (!(this_ptr[i] == it_ptr[i])) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(int).name())
            {
                const int* const this_ptr = (static_cast<int*>(item->data));
                const int* const it_ptr = (static_cast<int*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<int>() != it.arraySize<int>()) { return false; }

                    for (size_t i = 0; i < arraySize<int>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(unsigned int).name())
            {
                const unsigned int* const this_ptr = (static_cast<unsigned int*>(item->data));
                const unsigned int* const it_ptr = (static_cast<unsigned int*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<unsigned int>() != it.arraySize<unsigned int>()) { return false; }

                    for (size_t i = 0; i < arraySize<unsigned int>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(long int).name())
            {
                const long int* const this_ptr = (static_cast<long int*>(item->data));
                const long int* const it_ptr = (static_cast<long int*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<long int>() != it.arraySize<long int>()) { return false; }

                    for (size_t i = 0; i < arraySize<long int>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(unsigned long int).name())
            {
                const unsigned long int* const this_ptr = (static_cast<unsigned long int*>(item->data));
                const unsigned long int* const it_ptr = (static_cast<unsigned long int*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<unsigned long int>() != it.arraySize<unsigned long int>()) { return false; }

                    for (size_t i = 0; i < arraySize<unsigned long int>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(long long int).name())
            {
                const long long int* const this_ptr = (static_cast<long long int*>(item->data));
                const long long int* const it_ptr = (static_cast<long long int*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<long long int>() != it.arraySize<long long int>()) { return false; }

                    for (size_t i = 0; i < arraySize<long long int>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(unsigned long long int).name())
            {
                const unsigned long long int* const this_ptr = (static_cast<unsigned long long int*>(item->data));
                const unsigned long long int* const it_ptr = (static_cast<unsigned long long int*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<unsigned long long int>() != it.arraySize<unsigned long long int>()) { return false; }

                    for (size_t i = 0; i < arraySize<unsigned long long int>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(short int).name())
            {
                const short int* const this_ptr = (static_cast<short int*>(item->data));
                const short int* const it_ptr = (static_cast<short int*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<short int>() != it.arraySize<short int>()) { return false; }

                    for (size_t i = 0; i < arraySize<short int>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(unsigned short int).name())
            {
                const unsigned short int* const this_ptr = (static_cast<unsigned short int*>(item->data));
                const unsigned short int* const it_ptr = (static_cast<unsigned short int*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<unsigned short int>() != it.arraySize<unsigned short int>()) { return false; }

                    for (size_t i = 0; i < arraySize<unsigned short int>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(size_t).name())
            {
                const size_t* const this_ptr = (static_cast<size_t*>(item->data));
                const size_t* const it_ptr = (static_cast<size_t*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<size_t>() != it.arraySize<size_t>()) { return false; }

                    for (size_t i = 0; i < arraySize<size_t>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(char).name() || *minusedStr == typeid(char*).name())
            {
                const char* const this_ptr = (static_cast<char*>(item->data));
                const char* const it_ptr = (static_cast<char*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<char>() != it.arraySize<char>()) { return false; }

                    for (size_t i = 0; i < arraySize<char>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(unsigned char).name())
            {
                const unsigned char* const this_ptr = (static_cast<unsigned char*>(item->data));
                const unsigned char* const it_ptr = (static_cast<unsigned char*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<unsigned char>() != it.arraySize<unsigned char>()) { return false; }

                    for (size_t i = 0; i < arraySize<unsigned char>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(wchar_t).name())
            {
                const wchar_t* const this_ptr = (static_cast<wchar_t*>(item->data));
                const wchar_t* const it_ptr = (static_cast<wchar_t*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<wchar_t>() != it.arraySize<wchar_t>()) { return false; }

                    for (size_t i = 0; i < arraySize<wchar_t>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(float).name())
            {
                const float* const this_ptr = (static_cast<float*>(item->data));
                const float* const it_ptr = (static_cast<float*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<float>() != it.arraySize<float>()) { return false; }

                    for (size_t i = 0; i < arraySize<float>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(double).name())
            {
                const double* const this_ptr = (static_cast<double*>(item->data));
                const double* const it_ptr = (static_cast<double*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<double>() != it.arraySize<double>()) { return false; }

                    for (size_t i = 0; i < arraySize<double>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }

            else if (*minusedStr == typeid(long double).name())
            {
                const long double* const this_ptr = (static_cast<long double*>(item->data));
                const long double* const it_ptr = (static_cast<long double*>(it.item->data));

                if (item->isArray == true)
                {
                    if (arraySize<long double>() != it.arraySize<long double>()) { return false; }

                    for (size_t i = 0; i < arraySize<long double>(); i++)
                    {
                        if (this_ptr[i] != it_ptr[i]) { return false; }
                    }

                    return true;
                }

                else { return (*this_ptr == *it_ptr); }
            }
        }


        return false;
    }


    bool var::operator!=(const var& it)
    {
        is_item_null();
        is_item_dataType_null();


        if (contains(*item->dataType, *dataType_Spec().triv) == true)
        {
            return !(*this == it);
        }


        return true;
    }


    bool var::operator>(const var& it)
    {
        is_item_null();
        is_item_dataType_null();


        if (contains(*item->dataType, *dataType_Spec().triv) == true)
        {
            std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

            if (*minusedStr == typeid(var).name())
            {
                var* const this_ptr = (static_cast<var*>(item->data));
                const var* const it_ptr = (static_cast<var*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(int).name())
            {
                const int* const this_ptr = (static_cast<int*>(item->data));
                const int* const it_ptr = (static_cast<int*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned int).name())
            {
                const unsigned int* const this_ptr = (static_cast<unsigned int*>(item->data));
                const unsigned int* const it_ptr = (static_cast<unsigned int*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(long int).name())
            {
                const long int* const this_ptr = (static_cast<long int*>(item->data));
                const long int* const it_ptr = (static_cast<long int*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned long int).name())
            {
                const unsigned long int* const this_ptr = (static_cast<unsigned long int*>(item->data));
                const unsigned long int* const it_ptr = (static_cast<unsigned long int*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(long long int).name())
            {
                const long long int* const this_ptr = (static_cast<long long int*>(item->data));
                const long long int* const it_ptr = (static_cast<long long int*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned long long int).name())
            {
                const unsigned long long int* const this_ptr = (static_cast<unsigned long long int*>(item->data));
                const unsigned long long int* const it_ptr = (static_cast<unsigned long long int*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(short int).name())
            {
                const short int* const this_ptr = (static_cast<short int*>(item->data));
                const short int* const it_ptr = (static_cast<short int*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned short int).name())
            {
                const unsigned short int* const this_ptr = (static_cast<unsigned short int*>(item->data));
                const unsigned short int* const it_ptr = (static_cast<unsigned short int*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(size_t).name())
            {
                const size_t* const this_ptr = (static_cast<size_t*>(item->data));
                const size_t* const it_ptr = (static_cast<size_t*>(item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(char).name() || *minusedStr == typeid(char*).name())
            {
                const char* const this_ptr = (static_cast<char*>(item->data));
                const char* const it_ptr = (static_cast<char*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned char).name())
            {
                const unsigned char* const this_ptr = (static_cast<unsigned char*>(item->data));
                const unsigned char* const it_ptr = (static_cast<unsigned char*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(wchar_t).name())
            {
                const wchar_t* const this_ptr = (static_cast<wchar_t*>(item->data));
                const wchar_t* const it_ptr = (static_cast<wchar_t*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(float).name())
            {
                const float* const this_ptr = (static_cast<float*>(item->data));
                const float* const it_ptr = (static_cast<float*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(double).name())
            {
                const double* const this_ptr = (static_cast<double*>(item->data));
                const double* const it_ptr = (static_cast<double*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }

            else if (*minusedStr == typeid(long double).name())
            {
                const long double* const this_ptr = (static_cast<long double*>(item->data));
                const long double* const it_ptr = (static_cast<long double*>(it.item->data));

                return (*this_ptr > *it_ptr);
            }
        }


        return false;
    }


    bool var::operator<(const var& it)
    {
        is_item_null();
        is_item_dataType_null();


        if (contains(*item->dataType, *dataType_Spec().triv) == true)
        {
            return !(*this > it);
        }


        return false;
    }


    bool var::operator>=(const var& it)
    {
        is_item_null();
        is_item_dataType_null();


        if (contains(*item->dataType, *dataType_Spec().triv) == true)
        {
            std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

            if (*minusedStr == typeid(var).name())
            {
                var* const this_ptr = (static_cast<var*>(item->data));
                const var* const it_ptr = (static_cast<var*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(int).name())
            {
                const int* const this_ptr = (static_cast<int*>(item->data));
                const int* const it_ptr = (static_cast<int*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned int).name())
            {
                const unsigned int* const this_ptr = (static_cast<unsigned int*>(item->data));
                const unsigned int* const it_ptr = (static_cast<unsigned int*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(long int).name())
            {
                const long int* const this_ptr = (static_cast<long int*>(item->data));
                const long int* const it_ptr = (static_cast<long int*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned long int).name())
            {
                const unsigned long int* const this_ptr = (static_cast<unsigned long int*>(item->data));
                const unsigned long int* const it_ptr = (static_cast<unsigned long int*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(long long int).name())
            {
                const long long int* const this_ptr = (static_cast<long long int*>(item->data));
                const long long int* const it_ptr = (static_cast<long long int*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned long long int).name())
            {
                const unsigned long long int* const this_ptr = (static_cast<unsigned long long int*>(item->data));
                const unsigned long long int* const it_ptr = (static_cast<unsigned long long int*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(short int).name())
            {
                const short int* const this_ptr = (static_cast<short int*>(item->data));
                const short int* const it_ptr = (static_cast<short int*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned short int).name())
            {
                const unsigned short int* const this_ptr = (static_cast<unsigned short int*>(item->data));
                const unsigned short int* const it_ptr = (static_cast<unsigned short int*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(size_t).name())
            {
                const size_t* const this_ptr = (static_cast<size_t*>(item->data));
                const size_t* const it_ptr = (static_cast<size_t*>(item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(char).name() || *minusedStr == typeid(char*).name())
            {
                const char* const this_ptr = (static_cast<char*>(item->data));
                const char* const it_ptr = (static_cast<char*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned char).name())
            {
                const unsigned char* const this_ptr = (static_cast<unsigned char*>(item->data));
                const unsigned char* const it_ptr = (static_cast<unsigned char*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(wchar_t).name())
            {
                const wchar_t* const this_ptr = (static_cast<wchar_t*>(item->data));
                const wchar_t* const it_ptr = (static_cast<wchar_t*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(float).name())
            {
                const float* const this_ptr = (static_cast<float*>(item->data));
                const float* const it_ptr = (static_cast<float*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(double).name())
            {
                const double* const this_ptr = (static_cast<double*>(item->data));
                const double* const it_ptr = (static_cast<double*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }

            else if (*minusedStr == typeid(long double).name())
            {
                const long double* const this_ptr = (static_cast<long double*>(item->data));
                const long double* const it_ptr = (static_cast<long double*>(it.item->data));

                return (*this_ptr >= *it_ptr);
            }
        }


        return false;
    }


    bool var::operator<=(const var& it)
    {
        is_item_null();
        is_item_dataType_null();


        if (contains(*item->dataType, *dataType_Spec().triv) == true)
        {
            std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

            if (*minusedStr == typeid(var).name())
            {
                var* const this_ptr = (static_cast<var*>(item->data));
                const var* const it_ptr = (static_cast<var*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(int).name())
            {
                const int* const this_ptr = (static_cast<int*>(item->data));
                const int* const it_ptr = (static_cast<int*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned int).name())
            {
                const unsigned int* const this_ptr = (static_cast<unsigned int*>(item->data));
                const unsigned int* const it_ptr = (static_cast<unsigned int*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(long int).name())
            {
                const long int* const this_ptr = (static_cast<long int*>(item->data));
                const long int* const it_ptr = (static_cast<long int*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned long int).name())
            {
                const unsigned long int* const this_ptr = (static_cast<unsigned long int*>(item->data));
                const unsigned long int* const it_ptr = (static_cast<unsigned long int*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(long long int).name())
            {
                const long long int* const this_ptr = (static_cast<long long int*>(item->data));
                const long long int* const it_ptr = (static_cast<long long int*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned long long int).name())
            {
                const unsigned long long int* const this_ptr = (static_cast<unsigned long long int*>(item->data));
                const unsigned long long int* const it_ptr = (static_cast<unsigned long long int*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(short int).name())
            {
                const short int* const this_ptr = (static_cast<short int*>(item->data));
                const short int* const it_ptr = (static_cast<short int*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned short int).name())
            {
                const unsigned short int* const this_ptr = (static_cast<unsigned short int*>(item->data));
                const unsigned short int* const it_ptr = (static_cast<unsigned short int*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(size_t).name())
            {
                const size_t* const this_ptr = (static_cast<size_t*>(item->data));
                const size_t* const it_ptr = (static_cast<size_t*>(item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(char).name() || *minusedStr == typeid(char*).name())
            {
                const char* const this_ptr = (static_cast<char*>(item->data));
                const char* const it_ptr = (static_cast<char*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(unsigned char).name())
            {
                const unsigned char* const this_ptr = (static_cast<unsigned char*>(item->data));
                const unsigned char* const it_ptr = (static_cast<unsigned char*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(wchar_t).name())
            {
                const wchar_t* const this_ptr = (static_cast<wchar_t*>(item->data));
                const wchar_t* const it_ptr = (static_cast<wchar_t*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(float).name())
            {
                const float* const this_ptr = (static_cast<float*>(item->data));
                const float* const it_ptr = (static_cast<float*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(double).name())
            {
                const double* const this_ptr = (static_cast<double*>(item->data));
                const double* const it_ptr = (static_cast<double*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }

            else if (*minusedStr == typeid(long double).name())
            {
                const long double* const this_ptr = (static_cast<long double*>(item->data));
                const long double* const it_ptr = (static_cast<long double*>(it.item->data));

                return (*this_ptr <= *it_ptr);
            }
        }


        return false;
    }


    template<typename T>
    var& var::operator+=(const T& it)
    {
        is_item_null();
        is_item_dataType_null();
        exception_func_for_operator_plus(it);


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

        if (item->isRef == false && item->isArray == false)
        {
            if (*minusedStr != typeid(T).name() || item->data == nullptr)
            {
                if (item->data == nullptr)
                {
                    item->data = (void*) malloc(sizeof(T));
                    item->byte_size = sizeof(T);
                }

                if (sizeof(T) >= item->byte_size)
                {
                    getDataType(it, true, MathOperators::plus);
                    *item->dataType = (typeid(T).name() + *dataType_Spec().triv);
                    item->byte_size = sizeof(T);
                }

                else if (sizeof(T) < item->byte_size)
                {
                    getDataType(it, false, MathOperators::plus);
                }
            }

            else if (*minusedStr == typeid(T).name() && item->data != nullptr)
            {
                T temp = *static_cast<T*>(item->data) + it;
                memcpy(item->data, &temp, sizeof(T));
            }
        }

        else if (item->isRef == true && *minusedStr != typeid(char*).name() && *minusedStr != typeid(char).name() && *minusedStr != typeid(unsigned char).name() && *minusedStr != typeid(wchar_t).name())
        {
            if (*minusedStr == typeid(T).name())
            {
                T temp = *static_cast<T*>(item->data) + it;
                memcpy(item->data, &temp, sizeof(T));
            }
        }

        return *this;
    }


    var& var::operator+=(const var& it)
    {
        is_item_null();
        is_item_dataType_null();


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));
        std::unique_ptr<std::string> itminusedStr (new std::string(strMinus(*it.item->dataType, *dataType_Spec().triv)));

        if ((*minusedStr == typeid(char*).name() && *itminusedStr != typeid(char*).name()) || (*minusedStr != typeid(char*).name() && *itminusedStr == typeid(char*).name()))
        {
            std::unique_ptr<std::string> errorMessage (new std::string());
            
            *errorMessage.get() = "error: Invalid conversion from '";
            *errorMessage.get() += *minusedStr.get();
            *errorMessage.get() += "' to '";
            *errorMessage.get() += *itminusedStr.get();
        *errorMessage.get() += "'";
            
            throw std::runtime_error(*errorMessage.get());
        }


        if (item->isRef == false && item->isArray == false)
        {
            if (item->dataType->empty() == true)
            {
                item->data = (void*) malloc(it.item->byte_size);
                *item->dataType = *it.item->dataType;
            }

            if (it.item->byte_size >= item->byte_size && *itminusedStr != typeid(char*).name() && *itminusedStr != typeid(unsigned char).name() && *itminusedStr != typeid(wchar_t).name() && *itminusedStr != typeid(char).name())
            {
                getDataTypeForVar(it, true, MathOperators::plus);
                *item->dataType = *it.item->dataType;
                item->byte_size = it.item->byte_size;
            }

            else if (it.item->byte_size < item->byte_size && *itminusedStr != typeid(char*).name() && *itminusedStr != typeid(unsigned char).name() &&*itminusedStr != typeid(wchar_t).name() && *itminusedStr != typeid(char).name())
            {
                getDataTypeForVar(it, false, MathOperators::plus);
            }

            else if (*itminusedStr ==  typeid(char*).name() && *minusedStr == typeid(char*).name())
            {
                char* data_temp = (char*) malloc(strlen(static_cast<char*>(item->data)) + 1);
                arrcpy(data_temp, static_cast<char*>(item->data), strlen(static_cast<char*>(item->data)) + 1);

                if (item->data != nullptr) { free((void*) item->data); }
                item->data = nullptr;

                const char* temp = static_cast<char*>(it.item->data);
                char* new_data = c_unit(data_temp, temp);
                free(data_temp);

                new_data[strlen(new_data)] = '\0';
                item->data = new_data;
            }
        }

        else if (item->isRef == true && *minusedStr != typeid(char*).name() && *minusedStr != typeid(unsigned char).name() && *minusedStr != typeid(wchar_t).name() && *minusedStr != typeid(char).name())
        {
            if (*item->dataType == *it.item->dataType)
            {
                getDataTypeForVar(it, false, MathOperators::plus);
            }
        }

        return *this;
    }


    var& var::operator+=(const std::string& it)
    {
        is_item_null();
        is_item_dataType_null();

        if (strMinus(*item->dataType, *dataType_Spec().triv) != typeid(char*).name())
        {
            if (item->dataType->empty() == false)
            {
                std::unique_ptr<std::string> errorMessage (new std::string());
                
                *errorMessage.get() = "error: Invalid conversion from '";
                *errorMessage.get() += strMinus(*item->dataType, *dataType_Spec().triv);
                *errorMessage.get() += "' to '";
                *errorMessage.get() += typeid(char*).name();
                *errorMessage.get() += "'";
                
                throw std::runtime_error(*errorMessage.get());
            }
        }


        if (item->isRef == false && item->isArray == false)
        {
            if (item->dataType->empty() == true) { *item->dataType = (typeid(char*).name() + *dataType_Spec().triv); }

            char* temp = new char[strlen(static_cast<char*>(item->data)) + it.size() + 1];
            arrcpy(temp, static_cast<char*>(item->data), strlen(static_cast<char*>(item->data))+1);

            size_t sz = strlen(static_cast<char*>(item->data));

            if (item->data != nullptr) { free((void*) item->data); }
            item->data = nullptr;

            size_t count = 0;
            for (std::uint64_t i = sz; i < (sz + it.size()); i++)
            {
                temp[i] = it[count];
                count += 1;
            }

            temp[sz + it.size()] = '\0';
            item->data = temp;
        }

        return *this;
    }

    var operator+(const var& it, const var& it2)
    {
        var temp = it;
        temp += it2;
        return temp;
    }


    template<typename T>
    var& var::operator-=(const T& it)
    {
        is_item_null();
        is_item_dataType_null();
        exception_func_for_operators(it, MathOperators::minus);


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

        if (item->isRef == false && item->isArray == false)
        {
            if (*minusedStr != typeid(T).name())
            {
                if (item->data == nullptr) { item->data = (void*) malloc(sizeof(T)); }

                if (sizeof(T) >= item->byte_size)
                {
                    getDataType(it, true, MathOperators::minus);
                    *item->dataType = (typeid(T).name() + *dataType_Spec().triv);
                    item->byte_size = sizeof(T);
                }

                else if (sizeof(T) < item->byte_size)
                {
                    getDataType(it, false, MathOperators::minus);
                }
            }

            else if (*minusedStr == typeid(T).name())
            {
                T temp = *static_cast<T*>(item->data) - it;
                memcpy(item->data, &temp, sizeof(T));
            }
        }

        else if (item->isRef == true)
        {
            if (*minusedStr == typeid(T).name())
            {
                var temp = *static_cast<T*>(item->data) - it;
                memcpy(item->data, temp.item->data, sizeof(T));
            }
        }

        return *this;
    }


    var& var::operator-=(const var& it)
    {
        is_item_null();
        is_item_dataType_null();
        exception_func_for_operators_var(it, MathOperators::minus);


        if (item->isRef == false && item->isArray == false)
        {
            if (item->data == nullptr)
            {
                item->data = (void*) malloc(it.item->byte_size);
                *item->dataType = *it.item->dataType;
            }

            if (it.item->byte_size >= item->byte_size)
            {
                getDataTypeForVar(it, true, MathOperators::minus);
                *item->dataType = *it.item->dataType;
                item->byte_size = it.item->byte_size;
            }

            else if (it.item->byte_size < item->byte_size)
            {
                getDataTypeForVar(it, false, MathOperators::minus);
            }
        }

        else if (item->isRef == true)
        {
            if (*item->dataType == *it.item->dataType)
            {
                getDataTypeForVar(it, false, MathOperators::minus);
            }
        }

        return *this;
    }


    var operator-(const var& it, const var& it2)
    {
        var temp = it;
        temp -= it2;
        return temp;
    }


    template<typename T>
    var& var::operator*=(const T& it)
    {
        is_item_null();
        is_item_dataType_null();
        exception_func_for_operators(it, MathOperators::times);


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

        if (item->isRef == false && item->isArray == false)
        {
            if (*minusedStr != typeid(T).name() || item->data == nullptr)
            {
                if (item->data == nullptr) { item->data = (void*) malloc(sizeof(T)); }

                if (sizeof(T) >= item->byte_size)
                {
                    getDataType(it, true, MathOperators::times);
                    *item->dataType = (typeid(T).name() + *dataType_Spec().triv);
                    item->byte_size = sizeof(T);
                }

                else if (sizeof(T) < item->byte_size)
                {
                    getDataType(it, false, MathOperators::times);
                }
            }

            else if (*minusedStr == typeid(T).name())
            {
                T temp = *static_cast<T*>(item->data) * it;
                memcpy(item->data, &temp, sizeof(T));
            }
        }

        else if (item->isRef == true)
        {
            if (*minusedStr == typeid(T).name())
            {
                var temp = *static_cast<T*>(item->data) * it;
                memcpy(item->data, temp.item->data, sizeof(T));
            }
        }

        return *this;
    }


    var& var::operator*=(const var& it)
    {
        is_item_null();
        is_item_dataType_null();
        exception_func_for_operators_var(it, MathOperators::times);

        if (item->isRef == false && item->isArray == false)
        {
            if (item->data == nullptr)
            {
                item->data = (void*) malloc(it.item->byte_size);
                *item->dataType = *it.item->dataType;
            }

            if (it.item->byte_size >= item->byte_size)
            {
                getDataTypeForVar(it, true, MathOperators::times);
                *item->dataType = *it.item->dataType;
            }

            else if (it.item->byte_size < item->byte_size)
            {
                getDataTypeForVar(it, false, MathOperators::times);
            }
        }

        else if (item->isRef == true)
        {
            if (*item->dataType == *it.item->dataType)
            {
                getDataTypeForVar(it, false, MathOperators::times);
            }
        }

        return *this;
    }


    var operator*(const var& it, const var& it2)
    {
        var temp = it;
        temp *= it2;
        return temp;
    }


    template<typename T>
    var& var::operator/=(const T& it)
    {
        is_item_null();
        is_item_dataType_null();
        exception_func_for_operators(it, MathOperators::devide);


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

        if (item->isRef == false && item->isArray == false)
        {
            if (*minusedStr != typeid(T).name() || item->data == nullptr)
            {
                if (item->data == nullptr) { item->data = (void*) malloc(sizeof(T)); }

                if (sizeof(T) >= item->byte_size)
                {
                    getDataType(it, true, MathOperators::devide);
                    *item->dataType = (typeid(T).name() + *dataType_Spec().triv);
                    item->byte_size = sizeof(T);
                }

                else if (sizeof(T) < item->byte_size)
                {
                    getDataType(it, false, MathOperators::devide);
                }
            }

            else if (*minusedStr == typeid(T).name())
            {
                T temp = *static_cast<T*>(item->data) / it;
                memcpy(item->data, &temp, sizeof(T));
            }
        }

        else if (item->isRef == true)
        {
            if (*minusedStr == typeid(T).name())
            {
                var temp = *static_cast<T*>(item->data) / it;
                memcpy(item->data, temp.item->data, sizeof(T));
            }
        }

        return *this;
    }


    var& var::operator/=(const var& it)
    {
        is_item_null();
        is_item_dataType_null();
        exception_func_for_operators_var(it, MathOperators::devide);


        if (item->isRef == false && item->isArray == false)
        {
            if (item->data == nullptr)
            {
                item->data = (void*) malloc(it.item->byte_size);
                *item->dataType = *it.item->dataType;
            }

            if (it.item->byte_size >= item->byte_size)
            {
                getDataTypeForVar(it, true, MathOperators::devide);
                *item->dataType = *it.item->dataType;
                item->byte_size = it.item->byte_size;
            }

            else if (it.item->byte_size < item->byte_size)
            {
                getDataTypeForVar(it, false, MathOperators::devide);
            }
        }

        else if (item->isRef == true)
        {
            if (*item->dataType == *it.item->dataType)
            {
                getDataTypeForVar(it, false, MathOperators::devide);
            }
        }

        return *this;
    }


    var operator/(const var& it, const var& it2)
    {
        var temp = it;
        temp /= it2;
        return temp;
    }


    template<typename T>
    var& var::operator%=(const T& it)
    {
        is_item_null();
        is_item_dataType_null();
        exception_func_for_modulo(it);


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

        if ((item->isRef == false && item->isArray == false) || (item->isRef == true && item->isArray == false))
        {
            if (*minusedStr == typeid(int).name()) { *(static_cast<int*>(item->data)) %= it; }

            else if (*minusedStr == typeid(unsigned int).name()) { *(static_cast<unsigned int*>(item->data)) %= it; }

            else if (*minusedStr == typeid(size_t).name()) { *(static_cast<size_t*>(item->data)) %= it; }

            else if (*minusedStr == typeid(short int).name()) { *(static_cast<short int*>(item->data)) %= it; }

            else if (*minusedStr == typeid(unsigned short int).name()) { *(static_cast<unsigned short int*>(item->data)) %= it; }

            else if (*minusedStr == typeid(long int).name()) { *(static_cast<long int*>(item->data)) %= it; }

            else if (*minusedStr == typeid(unsigned long int).name()) { *(static_cast<unsigned long int*>(item->data)) %= it; }

            else if (*minusedStr == typeid(long long int).name()) { *(static_cast<long long int*>(item->data)) %= it; }

            else if (*minusedStr == typeid(unsigned long long int).name()) { *(static_cast<unsigned long long int*>(item->data)) %= it; }
        }

        return *this;
    }


    var& var::operator%=(const var& it)
    {
        is_item_null();
        is_item_dataType_null();
        exception_func_for_modulo(it);


        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

        if (((item->isRef == false && item->isArray == false) || (item->isRef == true && item->isArray == false)) && ((it.item->isRef == false && it.item->isArray == false) || (it.item->isRef == true && it.item->isArray == false)))
        {
            if (*minusedStr == typeid(int).name()) { operatorModulo_func(*(static_cast<int*>(item->data)), it); }

            else if (*minusedStr == typeid(unsigned int).name()) { operatorModulo_func(*(static_cast<unsigned int*>(item->data)), it); }

            else if (*minusedStr == typeid(size_t).name()) { operatorModulo_func(*(static_cast<size_t*>(item->data)), it); }

            else if (*minusedStr == typeid(short int).name()) { operatorModulo_func(*(static_cast<short int*>(item->data)), it); }

            else if (*minusedStr == typeid(unsigned short int).name()) { operatorModulo_func(*(static_cast<unsigned short int*>(item->data)), it); }

            else if (*minusedStr == typeid(long int).name()) { operatorModulo_func(*(static_cast<long int*>(item->data)), it); }

            else if (*minusedStr == typeid(unsigned long int).name()) { operatorModulo_func(*(static_cast<unsigned long int*>(item->data)), it); }

            else if (*minusedStr == typeid(long long int).name()) { operatorModulo_func(*(static_cast<long long int*>(item->data)), it); }

            else if (*minusedStr == typeid(unsigned long long int).name()) { operatorModulo_func(*(static_cast<unsigned long long int*>(item->data)), it); }
        }

        return *this;
    }


    var operator%(const var& it, const var& it2)
    {
        var temp = it;
        temp %= it2;
        return temp;
    }


    template<typename T>
    T& var::value(const size_t& num) const
    {
        if (item == nullptr || item->dataType->empty() == true)
        {
            throw std::runtime_error("bug: No accessible memory");
        }
        
        else if (num > (item->byte_size / sizeof(T)))
        {
            throw std::out_of_range("bug: Array index out of range");
        }


        T* const temp = static_cast<T*>(item->data);
        return temp[num];
    }


    template<typename T>
    T* var::values() const
    {
        if (item == nullptr || item->dataType->empty() == true)
        {
            throw std::runtime_error("bug: No accessible memory");
        }
        
        
        T* const temp = static_cast<T*>(item->data);
        return temp;
    }


    std::string var::type() const
    {
        std::unique_ptr<std::string> retValue (new std::string());


        if (item == nullptr || item->dataType->empty() == true)
        {
            *retValue.get() = "0";
        }
        
        else if (item->dataType->empty() != true)
        {
            if (contains(*item->dataType, *dataType_Spec().nontriv) == true) { *retValue.get() = strMinus(*item->dataType, *dataType_Spec().nontriv); }
            else { *retValue.get() = strMinus(*item->dataType, *dataType_Spec().triv); }
        }


        return *retValue.get();
    }


    bool var::IsArray() const
    {
        bool retValue = false;


        if (item == nullptr || item->dataType->empty() == true || item->isArray == false) { retValue = false; }
        else if (item->dataType->empty() != true) { retValue = item->isArray; }


        return retValue;
    }


    bool var::IsTrivial() const
    {
        bool retValue = false;


        if (item == nullptr || item->dataType->empty() == true) { retValue = false; }
        else if (item->dataType->empty() != true) { retValue = (contains(*item->dataType, *dataType_Spec().triv)); }


        return retValue;
    }


    size_t var::memsize() const
    {
        size_t retValue = 0;


        if (item == nullptr || item->dataType->empty() == true) { retValue = 0; }
        else if (item->dataType->empty() != true) { retValue = item->byte_size; }


        return retValue;
    }


    bool var::IsReffering() const
    {
        bool retValue = false;


        if (item == nullptr || item->dataType->empty() == true) { retValue = false; }
        else if (item->dataType->empty() != true) { retValue = item->isRef; }


        return retValue;
    }


    void var::getvar(const char& to)
    {
        is_item_null();
        is_item_dataType_null();

        std::string s;
        std::getline(std::cin, s, to);
        std::cin.clear();
        std::cin.sync();

        bool isNumber = ((sentence) s).isNumber();


        if (isNumber == true)
        {
            if (item->data != nullptr && contains(*item->dataType, *dataType_Spec().nontriv) == false && item->isRef == false && item->isArray == false) { free((void*) item->data); }
            else if (item->data != nullptr && item->isRef == false && item->isArray == true && contains(*item->dataType, *dataType_Spec().nontriv) == false) { deleteArray(); }
            item->data = nullptr;

            long double temp = std::strtold(s.c_str(), nullptr);

            item->data = (void*) malloc(sizeof(long double));
            memcpy(item->data, &temp, sizeof(long double));
            *item->dataType = (typeid(long double).name() + *dataType_Spec().triv);
            item->byte_size = sizeof(long double);
        }

        else if (s.size() == 1 && isNumber == false)
        {
            if (item->data != nullptr && contains(*item->dataType, *dataType_Spec().nontriv) == false && item->isRef == false && item->isArray == false) { free((void*) item->data); }
            else if (item->data != nullptr && item->isRef == false && item->isArray == true && contains(*item->dataType, *dataType_Spec().nontriv) == false) { deleteArray(); }
            item->data = nullptr;

            item->data = (void*) malloc(1);
            memcpy(item->data, s.data(), 1);

            *item->dataType = (typeid(char).name() + *dataType_Spec().triv);
            item->byte_size = sizeof(char);
        }

        else if (s.size() > 1 && isNumber == false)
        {
            if (item->data != nullptr && contains(*item->dataType, *dataType_Spec().nontriv) == false && item->isRef == false && item->isArray == false) { free((void*) item->data); }
            else if (item->data != nullptr && item->isRef == false && item->isArray == true && contains(*item->dataType, *dataType_Spec().nontriv) == false) { deleteArray(); }
            item->data = nullptr;

            item->data = (void*) malloc(s.size() + 1);
            memcpy(item->data, s.data(), s.size());
            (static_cast<char*>(item->data))[s.size()] = '\0';

            *item->dataType = (typeid(char*).name() + *dataType_Spec().triv);
            item->byte_size = (s.size() + 1);
        }
    }


    void var::is_item_null()
    {
        if (item == nullptr) { item = new Data; }
    }

    void var::is_item_dataType_null()
    {
        if (item != nullptr && item->dataType == nullptr) { item->dataType = new std::string; }
    }


    std::ostream& operator<<(std::ostream& os, const var& obj)
    {
        if (obj.item == nullptr || obj.item->dataType->empty() == true) { return (os << "0"); }


        std::unique_ptr<std::string> minusedStr (new std::string(obj.strMinus(*obj.item->dataType, *var::dataType_Spec().triv)));


        if (*minusedStr == typeid(var).name())
        {
            const var* const temp = static_cast<var*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(var)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(int).name())
        {
            const int* const temp = static_cast<int*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(int)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(unsigned int).name())
        {
            const unsigned int* const temp = static_cast<unsigned int*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(unsigned int)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(long int).name())
        {
            const long int* const temp = static_cast<long int*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(long int)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(unsigned long int).name())
        {
            const unsigned long int* const temp = static_cast<unsigned long int*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(unsigned long int)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(long long int).name())
        {
            const long long int* const temp = static_cast<long long int*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(long long int)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(unsigned long long int).name())
        {
            const unsigned long long int* const temp = static_cast<unsigned long long int*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(unsigned long long int)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(char).name())
        {
            if (obj.item->isArray == false) { os << *(static_cast<char*>(obj.item->data)); }
        }

        else if (*minusedStr == typeid(char*).name())
        {
            os << static_cast<char*>(obj.item->data);
        }

        else if (*minusedStr == typeid(size_t).name())
        {
            const size_t* const temp = static_cast<size_t*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(size_t)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(short int).name())
        {
            const short int* const temp = static_cast<short int*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(short int)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(unsigned short int).name())
        {
            const unsigned short int* const temp = static_cast<unsigned short int*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(unsigned short int)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(unsigned char).name())
        {
            if (obj.item->isArray == false) { os << *(static_cast<unsigned char*>(obj.item->data)); }
        }

        else if (*minusedStr == typeid(wchar_t).name())
        {
            if (obj.item->isArray == false) { os << *(static_cast<wchar_t*>(obj.item->data)); }
        }

        else if (*minusedStr == typeid(float).name())
        {
            const float* const temp = static_cast<float*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(float)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(double).name())
        {
            const double* const temp = static_cast<double*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(double)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (*minusedStr == typeid(long double).name())
        {
            const long double* const temp = static_cast<long double*>(obj.item->data);

            if (obj.item->isArray == false) { os << *temp; }
            else if (obj.item->isArray == true)
            {
                for (size_t i = 0; i < (obj.item->byte_size / sizeof(long double)); i++)
                {
                    os << temp[i] << " ";
                }
            }
        }

        else if (obj.contains(*obj.item->dataType, *var::dataType_Spec().nontriv) == true)
        {
            os << obj.item->data;
        }

        os.flush();
        return os;
    }



    bool operator==(const std::string& s, const std::string& s2)
    {
        if (s.size() != s2.size()) { return false; }

        size_t k = s.size() - 1;
        for (size_t i = 0; i < s.size(); i++)
        {
            if (s[i] != s2[i] || s[k] != s2[k]) { return false; }
            k -= 1;
        }

        return true;
    }


    std::string var::strMinus(const std::string& it, const std::string& it2) const
    {
        if (it.size() == 0) { return std::string(); }
        else if (it2.size() == 0) { return it; }

        std::unique_ptr<std::string> str(new std::string(it.c_str(), it.size() - it2.size()));
        return *str.get();
    }


    bool var::contains(const std::string& it, const std::string& sub) const
    {
        std::size_t position = it.find(sub);
        return !(position == std::string::npos);
    }


    template<typename T>
    void var::getDataType(T it, const bool& mensiTyp, const enum MathOperators& op) //Jestli je dosavadni hodnota mensiho datoveho typu nez-li hodnota, kterou pricitame
    {
        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

        if (*minusedStr == typeid(int).name())
        {
            int temp = *(static_cast<int*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(int)); }
        }

        else if (*minusedStr == typeid(unsigned int).name())
        {
            unsigned int temp = *(static_cast<unsigned int*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(unsigned int)); }
        }

        else if (*minusedStr == typeid(long int).name())
        {
            long int temp = *(static_cast<long int*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(long int)); }
        }

        else if (*minusedStr == typeid(unsigned long int).name())
        {
            unsigned long int temp = *(static_cast<unsigned long int*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(unsigned long int)); }
        }

        else if (*minusedStr == typeid(long long int).name())
        {
            long long int temp = *(static_cast<long long int*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(long long int)); }
        }

        else if (*minusedStr == typeid(unsigned long long int).name())
        {
            unsigned long long int temp = *(static_cast<unsigned long long int*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(unsigned long long int)); }
        }

        else if (*minusedStr == typeid(size_t).name())
        {
            size_t temp = *(static_cast<size_t*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(size_t)); }
        }

        else if (*minusedStr == typeid(short int).name())
        {
            short int temp = *(static_cast<short int*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(short int)); }
        }

        else if (*minusedStr == typeid(unsigned short int).name())
        {
            unsigned short int temp = *(static_cast<unsigned short int*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(unsigned short int)); }
        }

        else if (*minusedStr == typeid(float).name())
        {
            float temp = *(static_cast<float*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(float)); }
        }

        else if (*minusedStr == typeid(double).name())
        {
            double temp = *(static_cast<double*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(double)); }
        }

        else if (*minusedStr == typeid(long double).name())
        {
            long double temp = *(static_cast<long double*>(item->data));
            doOperation(it, temp, op);

            if (mensiTyp == true)
            {
            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(sizeof(T));
            memcpy(item->data, &it, sizeof(T));
            }

            else if (mensiTyp == false) { memcpy(item->data, &temp, sizeof(long double)); }
        }
    }


    void var::getDataTypeForVar(var it, const bool& mensiTyp, const enum MathOperators op)
    {
        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));


        if (*minusedStr == typeid(int).name())
        {
            const size_t itSize = it.item->byte_size;
            int temp = *(static_cast<int*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(unsigned int).name())
        {
            const size_t itSize = it.item->byte_size;
            unsigned int temp = *(static_cast<unsigned int*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(long int).name())
        {
            const size_t itSize = it.item->byte_size;
            long int temp = *(static_cast<long int*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(unsigned long int).name())
        {
            const size_t itSize = it.item->byte_size;
            unsigned long int temp = *(static_cast<unsigned long int*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(long long int).name())
        {
            const size_t itSize = it.item->byte_size;
            long long int temp = *(static_cast<long long int*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(unsigned long long int).name())
        {
            const size_t itSize = it.item->byte_size;
            unsigned long long int temp = *(static_cast<unsigned long long int*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(size_t).name())
        {
            const size_t itSize = it.item->byte_size;
            size_t temp = *(static_cast<size_t*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(short int).name())
        {
            const size_t itSize = it.item->byte_size;
            short int temp = *(static_cast<short int*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(unsigned short int).name())
        {
            const size_t itSize = it.item->byte_size;
            unsigned short int temp = *(static_cast<unsigned short int*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(float).name())
        {
            const size_t itSize = it.item->byte_size;
            float temp = *(static_cast<float*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(double).name())
        {
            const size_t itSize = it.item->byte_size;
            double temp = *(static_cast<double*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }

        else if (*minusedStr == typeid(long double).name())
        {
            const size_t itSize = it.item->byte_size;
            long double temp = *(static_cast<long double*>(item->data));
            doOperationVar(temp, it, item->byte_size, itSize, op);
        }
    }


    template<typename T, typename T2>
    void var::doOperation(T& hl, T2& vedl, const enum MathOperators& op)
    {
        if (sizeof(T) >= sizeof(T2))
        {
            if (op == MathOperators::plus) { hl += vedl; }
            else if (op == MathOperators::minus)
            {
                hl -= vedl;
                hl *= -1;
            }
            else if (op == MathOperators::times) { hl *= vedl; }
            else if (op == MathOperators::devide)
            {
                T temp = vedl;
                temp /= hl;
                hl = temp;
            }
        }

        else if (sizeof(T) < sizeof(T2))
        {
            if (op == MathOperators::plus) { vedl += hl; }
            else if (op == MathOperators::minus) { vedl -= hl; }
            else if (op == MathOperators::times) { vedl *= hl; }
            else if (op == MathOperators::devide) { vedl /= hl; }
        }
    }


    template<typename T>
    void var::doOperationVar(T& hl, var& vedl, const size_t& thisSize, const size_t& vedlSize, const enum MathOperators op)
    {
        if (thisSize >= vedlSize)
        {
            T* val = new T;
            std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*vedl.item->dataType, *dataType_Spec().triv)));

            if (*minusedStr == typeid(int).name())
            {
                int temp = *(static_cast<int*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(unsigned int).name())
            {
                unsigned int temp = *(static_cast<unsigned int*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(long int).name())
            {
                long int temp = *(static_cast<long int*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(unsigned long int).name())
            {
                unsigned long int temp = *(static_cast<unsigned long int*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(long long int).name())
            {
                long long int temp = *(static_cast<long long int*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(unsigned long long int).name())
            {
                unsigned long long int temp = *(static_cast<unsigned long long int*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(size_t).name())
            {
                size_t temp = *(static_cast<size_t*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(short int).name())
            {
                short int temp = *(static_cast<short int*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(unsigned short int).name())
            {
                unsigned short int temp = *(static_cast<unsigned short int*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(float).name())
            {
                float temp = *(static_cast<float*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(double).name())
            {
                double temp = *(static_cast<double*>(vedl.item->data));
                *val = temp;
            }

            else if (*minusedStr == typeid(long double).name())
            {
                long double temp = *(static_cast<long double*>(vedl.item->data));
                *val = temp;
            }


            if (op == MathOperators::plus) { hl += *val; }
            else if (op == MathOperators::minus) { hl -= *val; }
            else if (op == MathOperators::times) { hl *= *val; }
            else if (op == MathOperators::devide) { hl /= *val; }

            delete val;
            memcpy(item->data, &hl, sizeof(T));
        }

        else if (thisSize < vedlSize)
        {
            if (op == MathOperators::plus) { vedl += hl; }
            else if (op == MathOperators::minus) { vedl -= hl; vedl *= -1; }
            else if (op == MathOperators::times) { vedl *= hl; }
            else if (op == MathOperators::devide)
            {
                std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*vedl.item->dataType, *dataType_Spec().triv)));

                if (item->data != nullptr) { free((void*) item->data); }
                item->data = (void*) malloc(vedlSize);


                if (*minusedStr == typeid(int).name())
                {
                    int temp = hl;
                    temp /= *(static_cast<int*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(unsigned int).name())
                {
                    unsigned int temp = hl;
                    temp /= *(static_cast<unsigned int*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(long int).name())
                {
                    long int temp = hl;
                    temp /= *(static_cast<long int*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(unsigned long int).name())
                {
                    unsigned long int temp = hl;
                    temp /= *(static_cast<unsigned long int*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(long long int).name())
                {
                    long long int temp = hl;
                    temp /= *(static_cast<long long int*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(unsigned long long int).name())
                {
                    unsigned long long int temp = hl;
                    temp /= *(static_cast<unsigned long long int*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(short int).name())
                {
                    short int temp = hl;
                    temp /= *(static_cast<short int*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(unsigned short int).name())
                {
                    unsigned short int temp = hl;
                    temp /= *(static_cast<unsigned short int*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(size_t).name())
                {
                    size_t temp = hl;
                    temp /= *(static_cast<size_t*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(float).name())
                {
                    float temp = hl;
                    temp /= *(static_cast<float*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(double).name())
                {
                    double temp = hl;
                    temp /= *(static_cast<double*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }

                else if (*minusedStr == typeid(long double).name())
                {
                    long double temp = hl;
                    temp /= *(static_cast<long double*>(vedl.item->data));
                    memcpy(item->data, &temp, vedlSize);
                }


                return;
            }

            if (item->data != nullptr) { free((void*) item->data); }
            item->data = (void*) malloc(vedlSize);
            memcpy(item->data, vedl.item->data, vedlSize);
        }
    }


    template<typename T>
    void var::operatorModulo_func(T& temp, const var& it)
    {
        std::unique_ptr<std::string> minusedStrIt (new std::string(strMinus(*it.item->dataType, *dataType_Spec().triv)));


        if (*minusedStrIt == typeid(int).name()) { temp %= *(static_cast<int*>(it.item->data)); }

        else if (*minusedStrIt == typeid(unsigned int).name()) { temp %= *(static_cast<unsigned int*>(it.item->data)); }

        else if (*minusedStrIt == typeid(size_t).name()) { temp %= *(static_cast<size_t*>(it.item->data)); }

        else if (*minusedStrIt == typeid(short int).name()) { temp %= *(static_cast<short int*>(it.item->data)); }

        else if (*minusedStrIt == typeid(unsigned short int).name()) { temp %= *(static_cast<unsigned short int*>(it.item->data)); }

        else if (*minusedStrIt == typeid(long int).name()) { temp %= *(static_cast<long int*>(it.item->data)); }

        else if (*minusedStrIt == typeid(unsigned long int).name()) {  temp %= *(static_cast<unsigned long int*>(it.item->data)); }

        else if (*minusedStrIt == typeid(long long int).name()) { temp %= *(static_cast<long long int*>(it.item->data)); }

        else if (*minusedStrIt == typeid(unsigned long long int).name()) { temp %= *(static_cast<unsigned long long int*>(it.item->data)); }


        memcpy(item->data, &temp, sizeof(T));
    }



    //Exception methods



    template<typename T>
    void var::exception_func_for_operator_plus(const T& it)
    {
        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

        if (*minusedStr == typeid(wchar_t).name() || *minusedStr == typeid(unsigned char).name() || *minusedStr == typeid(char*).name() || *minusedStr == typeid(char).name() || typeid(T).name() == typeid(wchar_t).name() ||
            typeid(T).name() == typeid(unsigned char).name() || typeid(T).name() == typeid(char*).name() ||
            typeid(T).name() == typeid(char).name())
        {
            std::unique_ptr<std::string> errorMessage (new std::string());
            
            if (*minusedStr == typeid(T).name())
            {
                *errorMessage.get() = "error: No matching for \"operator+\" for object of type '";
                *errorMessage.get() +=  typeid(T).name();
                *errorMessage.get() += "'";
            }
            
            else if (*minusedStr == typeid(T).name())
            {
                *errorMessage.get() = "error: Invalid conversion from '";
                *errorMessage.get() += *minusedStr.get();
                *errorMessage.get() += "' to '";
                *errorMessage.get() += typeid(T).name();;
                *errorMessage.get() += "'";
            }
            
            throw std::runtime_error(*errorMessage.get());
        }
    }


    template<typename T>
    void var::exception_func_for_operators(const T& it, const enum MathOperators op)
    {
        bool sbadType = false;
        bool itbadType = false;
        
        std::unique_ptr<std::string> errorMessage (new std::string());
        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));

        if (*minusedStr == typeid(wchar_t).name() || *minusedStr == typeid(unsigned char).name() || *minusedStr == typeid(char*).name() || *minusedStr == typeid(char).name()) { sbadType = true; }

        if (typeid(T).name() == typeid(wchar_t).name() || typeid(T).name() == typeid(unsigned char).name() || typeid(T).name() == typeid(char*).name() ||
            typeid(T).name() == typeid(char).name()) { itbadType = true; }

        if ((sbadType == true && itbadType == false) || (sbadType == false && itbadType == true))
        {
            *errorMessage.get() = "error: Invalid conversion from '";
            *errorMessage.get() += *minusedStr;
            *errorMessage.get() += "' to '";
            *errorMessage.get() += typeid(T).name();
            *errorMessage.get() += "'";

            throw std::runtime_error(*errorMessage.get());
        }

        else if (sbadType == true && itbadType == true)
        {
            if (op == MathOperators::minus)
            {
                *errorMessage.get() = "error: No matching for \"operator-\" for object of type '";
                *errorMessage.get() += *minusedStr;
                *errorMessage.get() += "'";
            }

            else if (op == MathOperators::times)
            {
                *errorMessage.get() = "error: No matching for \"operator*\" for object of type '";
                *errorMessage.get() += *minusedStr;
                *errorMessage.get() += "'";
            }

            else if (op == MathOperators::devide)
            {
                *errorMessage.get() = "error: No matching for \"operator/\" for object of type '";
                *errorMessage.get() += *minusedStr;
                *errorMessage.get() += "'";
            }

            throw std::runtime_error(*errorMessage.get());
        }
    }


    void var::exception_func_for_operators_var(const var& it, const enum MathOperators op)
    {
        bool sbadType = false;
        bool itbadType = false;
        
        std::unique_ptr<std::string> errorMessage (new std::string());
        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));
        std::unique_ptr<std::string> minusedStrIt (new std::string(strMinus(*it.item->dataType, *dataType_Spec().triv)));

        if (*minusedStr == typeid(wchar_t).name() || *minusedStr == typeid(unsigned char).name() || *minusedStr == typeid(char*).name() || *minusedStr == typeid(char).name()) { sbadType = true; }
        if (*minusedStrIt == typeid(wchar_t).name() || *minusedStrIt == typeid(unsigned char).name() || *minusedStrIt == typeid(char*).name() || *minusedStrIt == typeid(char).name()) { itbadType = true; }

        if ((sbadType == true && itbadType == false) || (sbadType == false && itbadType == true))
        {
            *errorMessage.get() = "error: Invalid conversion from '";
            *errorMessage.get() += *minusedStr;
            *errorMessage.get() += "' to '";
            *errorMessage.get() += it.type();
            *errorMessage.get() += "'";

            throw std::runtime_error(*errorMessage.get());
        }

        else if (sbadType == true && itbadType == true)
        {
            if (op == MathOperators::minus)
            {
                *errorMessage.get() = "error: No matching for \"operator-\" for object of type '";
                *errorMessage.get() += *minusedStr;
                *errorMessage.get() += "'";
            }

            else if (op == MathOperators::times)
            {
                *errorMessage.get() = "error: No matching for \"operator*\" for object of type '";
                *errorMessage.get() += *minusedStr;
                *errorMessage.get() += "'";
            }

            else if (op == MathOperators::devide)
            {
                *errorMessage.get() = "error: No matching for \"operator/\" for object of type '";
                *errorMessage.get() += *minusedStr;
                *errorMessage.get() += "'";
            }

            throw std::runtime_error(*errorMessage.get());
        }
    }


    void var::exception_func_for_modulo(const var& it)
    {
        bool sbadType = false;
        bool itbadType = false;
        
        std::unique_ptr<std::string> minusedStr (new std::string(strMinus(*item->dataType, *dataType_Spec().triv)));
        std::unique_ptr<std::string> minusedStrIt (new std::string(strMinus(*it.item->dataType, *dataType_Spec().triv)));


        if (*minusedStr == typeid(wchar_t).name() || *minusedStr == typeid(unsigned char).name() || *minusedStr == typeid(char*).name() || *minusedStr == typeid(char).name() ||
        *minusedStr == typeid(float).name() || *minusedStr == typeid(double).name() || *minusedStr == typeid(long double).name()) { sbadType = true; }

        if (*minusedStrIt == typeid(wchar_t).name() || *minusedStrIt == typeid(unsigned char).name() || *minusedStrIt == typeid(char*).name() || *minusedStrIt == typeid(char).name() ||
        *minusedStrIt == typeid(float).name() || *minusedStrIt == typeid(double).name() || *minusedStrIt == typeid(long double).name()) { itbadType = true; }

        if ((sbadType == true && itbadType == false) || (sbadType == false && itbadType == true) || (sbadType == true && itbadType == true))
        {
            std::unique_ptr<std::string> errorMessage (new std::string());
            
            *errorMessage.get() = "error: Invalid operands of types '";
            *errorMessage.get() += *minusedStr;
            *errorMessage.get() += "' and '";
            *errorMessage.get() += *minusedStrIt;
            *errorMessage.get() += "'";
            *errorMessage.get() += " to binary \"operator%\"";

            throw std::runtime_error(*errorMessage.get());
        }
    }
}



#endif  // VAR_H 
