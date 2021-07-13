#ifndef CONVERT_HPP
#define CONVERT_HPP

#include "sentence.hpp"
#include <cinttypes>



namespace aea
{
    class Convert
    {
        public:
            Convert() = default;
            Convert(const Convert& obj) = delete;
            Convert(Convert&& obj) = delete;
            ~Convert() = default;

            Convert& operator=(const Convert& obj) = delete;
            Convert& operator=(Convert&& obj) = delete;

            sentence to_sentence(std::int64_t number);
            sentence to_sentence(std::uint64_t number);
            sentence to_sentence(float number);
            sentence to_sentence(double number);

            std::int8_t to_int8(const sentence& str);
            std::uint8_t to_uint8(const sentence& str);

            std::int16_t to_int16(const sentence& str);
            std::uint16_t to_uint16(const sentence& str);

            std::int32_t to_int32(const sentence& str);
            std::uint32_t to_uint32(const sentence& str);

            std::int64_t to_int64(const sentence& str);
            std::uint64_t to_uint64(const sentence& str);
            
            float to_float(const sentence& str);
            double to_double(const sentence& str);
    };

    

    sentence Convert::to_sentence(std::int64_t number)
    {
        sentence result;
        bool minus = false;

        if (number < 0) 
        { 
            result = '-';
            minus = true; 
        }

        if (minus == false)
        {
            while (number >= 1)
            {
                result += (number % 10) + '0';
                number /= 10;
            }

            result.reverse();
        }

        else if (minus == true)
        {
            while (number < 0)
            {
                result += ((number * -1) % 10) + '0';
                number /= 10;
            }

            result.reverse(1);
        }

        return result;
    }


    sentence Convert::to_sentence(std::uint64_t number)
    {
        sentence result;
        while (number >= 1)
        {
            result += (number % 10) + '0';
            number /= 10;
        }

        result.reverse();
        return result;
    }


    sentence Convert::to_sentence(float number)
    {
        bool minus = false;
        if (number < 0) { minus = true; }

        aea::sentence result = std::move(to_sentence(std::int64_t(number)));

        if (minus == false) { number -= std::int32_t(number); }
        else if (minus == true) { number += std::int32_t(number) * -1; }

        std::int64_t fltNumbers = float(number * float(pow(10, __FLT_DIG__)));
        result += '.';

        aea::sentence temp = std::move(to_sentence(fltNumbers));
        if (minus == true) { temp.remove(0); }
        result += temp;

        return result;
    }


    sentence Convert::to_sentence(double number)
    {
        bool minus = false;
        if (number < 0) { minus = true; }

        aea::sentence result = std::move(to_sentence(std::int64_t(number)));

        if (minus == false) { number -= std::int64_t(number); }
        else if (minus == true) { number += std::int64_t(number) * -1; }

        std::int64_t fltNumbers = double(number * double(pow(10, __DBL_DIG__)));
        result += '.';

        aea::sentence temp = std::move(to_sentence(fltNumbers));
        if (minus == true) { temp.remove(0); }
        result += temp;

        return result;
    }


    std::int8_t Convert::to_int8(const sentence& str)
    {
        return std::int8_t(to_int64(str));
    }


    std::uint8_t Convert::to_uint8(const sentence& str)
    {
        return std::uint8_t(to_uint64(str));
    }


    std::int16_t Convert::to_int16(const sentence& str)
    {
        return std::int16_t(to_int64(str));
    }


    std::uint16_t Convert::to_uint16(const sentence& str)
    {
        return std::uint16_t(to_uint64(str));
    }


    std::int32_t Convert::to_int32(const sentence& str)
    {
        return std::int32_t(to_int64(str));
    }


    std::uint32_t Convert::to_uint32(const sentence& str)
    {
        return std::uint32_t(to_uint64(str));
    }


    std::int64_t Convert::to_int64(const sentence& str)
    {
        if (str.empty() == true || ((sentence*) &str)->isNumber() == false || ((sentence*) &str)->isDouble() == true)
        { 
            return 0; 
        }

        bool minus = false;
        if (str.at(0) == '-') { minus = true; }

        std::int64_t vys = 0;
        std::uint64_t on = str.size() - 1;

        for (std::uint64_t i = minus; i < str.size(); ++i)
        {
            short int temp = str.at(i) - '0';

            if (minus == false) { vys += temp * pow(10, on); }
            else if (minus == true) { vys -= temp * pow(10, on); }

            on -= 1;
        }

        if (minus == true) { vys /= 10; }
        return vys;
    }


    std::uint64_t Convert::to_uint64(const sentence& str)
    {
        if (str.empty() == true || str.at(0) == '-' || ((sentence*) &str)->isNumber() == false || ((sentence*) &str)->isDouble() == true) 
        { 
            return 0; 
        }

        std::uint64_t vys = 0;
        std::int64_t on = str.size() - 1;

        for (std::uint64_t i = 0; i < str.size(); ++i)
        {
            short int temp = str.at(i) - '0';
            vys += temp * pow(10, on);

            on -= 1;
        }

        return vys;
    }


    float Convert::to_float(const sentence& str)
    {
        if (str.empty() == true || ((sentence*) &str)->isNumber() == false)
        { 
            return 0; 
        }

        std::int64_t dotPosition = str.find('.');
        if (dotPosition == -1) { return float(to_int32(str)); }

        aea::sentence temp(dotPosition, ' ');
        aea::arrcopy(temp.get(), str.get(), dotPosition);

        float result = 0;
        result = to_int32(temp);

        temp.reset();
        temp = std::move(aea::sentence((str.size() - dotPosition - 1), ' '));
        aea::arrcopy(temp.get(), (str.first() + dotPosition + 1), (str.size() - dotPosition - 1));

        if (str.at(0) == '-') { result -= float(float(to_int32(temp)) / float(pow(10, (str.size() - dotPosition - 1)))); }
        else if (str.at(0) != '-') { result += float(float(to_int32(temp)) / float(pow(10, (str.size() - dotPosition - 1)))); }

        return result;
    }


    double Convert::to_double(const sentence& str)
    {
        if (str.empty() == true || ((sentence*) &str)->isNumber() == false)
        { 
            return 0; 
        }

        std::int64_t dotPosition = str.find('.');
        if (dotPosition == -1) { return double(to_int64(str)); }

        aea::sentence temp(dotPosition, ' ');
        aea::arrcopy(temp.get(), str.get(), dotPosition);

        double result = 0;
        result = to_int64(temp);

        temp.reset();
        temp = std::move(aea::sentence((str.size() - dotPosition - 1), ' '));
        aea::arrcopy(temp.get(), (str.first() + dotPosition + 1), (str.size() - dotPosition - 1));

        if (str.at(0) == '-') { result -= double(double(to_int64(temp)) / double(pow(10, (str.size() - dotPosition - 1)))); }
        else if (str.at(0) != '-') { result += double(double(to_int64(temp)) / double(pow(10, (str.size() - dotPosition - 1)))); }

        return result;
    }
}



#endif  // CONVERT_HPP