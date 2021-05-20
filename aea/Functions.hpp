#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <type_traits>



namespace aea
{
    char* c_unit(const char* it, const char* it2)
    {
        
        char* temp = new char[strlen(it) + strlen(it2) + 1];

        memcpy(temp, it, strlen(it));
        memcpy((temp + strlen(it)), it2, strlen(it2));

        temp[strlen(it) + strlen(it2)] = '\0';

        return temp;
    }


    template<typename FType, typename SType>
    void arrcpy(FType* it, const SType* it2, const std::uint64_t& sz)
    {
        if (sizeof(FType) == sizeof(SType) && std::is_trivial<FType>::value == true && std::is_trivial<SType>::value == true)
        {
            memcpy(it, it2, (sizeof(FType) * sz));
        }

        else
        {
            for (std::uint64_t i = 0; i < sz; i++)
            {
                it[i] = it2[i];
            }
        }
    }
}



#endif  // FUNCTIONS_H
