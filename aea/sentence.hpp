#ifndef SENTENCE_HPP
#define SENTENCE_HPP

#include "BasicContainer.hpp"
#include <math.h>
#include <limits>



namespace aea
{
    class sentence : public BasicContainer<char>
    {
        public:
            sentence() = default;
            sentence(char c);
            sentence(const char* str);
            sentence(const std::string& str);
            sentence(const std::uint64_t& size, char c);
            sentence(const std::initializer_list<char>& list);
            sentence(const sentence& obj);
            sentence(sentence&& obj);
            virtual ~sentence() = default;

            sentence& operator=(char c);
            sentence& operator=(const char* str);
            sentence& operator=(const std::string& str);
            sentence& operator=(const std::initializer_list<char>& list);
            sentence& operator=(const sentence& obj);
            sentence& operator=(sentence&& obj);

            operator std::string();

            sentence& operator+=(char c);
            sentence& operator+=(const sentence& obj);
            friend sentence operator+(const sentence& obj, const sentence& obj2);

            sentence& getInput(const char to = '\n');
            sentence& getInput(const std::uint64_t& size);
            bool equal(const sentence& obj) const;
            void insert(const std::uint64_t& position, char c);
            void remove(const std::uint64_t& position);
            void toUpper();
            void toLower();
            std::int64_t find(const char c, std::uint64_t* lastPosition = nullptr) const;
		    bool contains(sentence str);
		    bool contains(sentence str, const size_t& at);
            void reverse();
            void reverse(const std::uint64_t& from);
            long double toNumber();
            bool isNumber();
            bool isDouble();
            bool empty() const;

            friend sentence& operator<<(sentence& obj, const sentence& str);
            friend std::ostream& operator<<(std::ostream& os, const sentence& obj);


        protected:
            std::ostream& print(std::ostream& os) const;
    };



    sentence::sentence(char c)
    {
        this->begin = new char(c);
        this->end = this->begin;
    }


    sentence::sentence(const char* str)
    {
        const std::uint64_t size = strlen(str); 

        if (size > 0)
        {
            if (size == 1) { this->begin = new char; }
            else if (size > 1) { this->begin = new char[size]; } 
            this->end = (this->begin + size - 1);

            strncpy(this->begin, str, size);
        }
    }


    sentence::sentence(const std::string& str)
    {
        const std::uint64_t size = str.size(); 

        if (size > 0)
        {
            if (size == 1) { this->begin = new char; }
            else if (size > 1) { this->begin = new char[size]; }
            this->end = (this->begin + size - 1);

            strncpy(this->begin, str.data(), size);
        }
    }


    sentence::sentence(const std::uint64_t& size, char c)
    {
        if (size > 0)
        {
            if (size == 1) { this->begin = new char; }
            else if (size > 1) { this->begin = new char[size]; }

            this->end = (this->begin + size - 1);
            memset(this->begin, c, size);
        }
    }


    sentence::sentence(const std::initializer_list<char>& list) : BasicContainer<char>(list)
    {

    }

    
    sentence::sentence(const sentence& obj) : BasicContainer<char>(obj)
    {

    }


    sentence::sentence(sentence&& obj) : BasicContainer<char>(std::move(obj))
    {
        
    }


    sentence& sentence::operator=(char c)
    {
        this->reset();

        this->begin = new char(c);
        this->end = this->begin;

        return *this;
    }


    sentence& sentence::operator=(const char* str)
    {
        this->reset();

        if (str != nullptr)
        {
            const std::uint64_t size = strlen(str);

            if (size > 0)
            {
                if (size == 1) { this->begin = new char; }
                else if (size > 1) { this->begin = new char[size]; }

                this->end = (this->begin + size - 1);
                strncpy(this->begin, str, size);
            }
        }

        return *this;
    }


    sentence& sentence::operator=(const std::string& str)
    {
        this->reset();

        if (str.empty() == false)
        {
            const std::uint64_t size = str.size();

            if (size > 0)
            {
                if (size == 1) { this->begin = new char; }
                else if (size > 1) { this->begin = new char[size]; }

                this->end = (this->begin + size - 1);

                strncpy(this->begin, str.data(), size);
            }
        }

        return *this;
    }


    sentence& sentence::operator=(const std::initializer_list<char>& list)
    {
        this->reset();

        const std::uint64_t size = list.size();

        if (size > 0)
        {
            if (size == 1) { this->begin = new char; }
            else if (size > 1) { this->begin = new char[size]; }
            this->end = (this->begin + size - 1);

            for (std::uint64_t i = 0; i < size; ++i)
            {
                this->begin[i] = *list.begin() + i;
            }
        }

        return *this;
    }

    
    sentence& sentence::operator=(const sentence& obj)
    {
        if (obj.begin != nullptr)
        {
            this->reset();

            const std::uint64_t size = obj.size();

            if (size > 0)
            {
                if (size == 1) { this->begin = new char; }
                else if (size > 1) { this->begin = new char[size]; }

                this->end = (this->begin + size - 1);
                strncpy(this->begin, obj.begin, size);
            }
        }

        return *this;
    }


    sentence& sentence::operator=(sentence&& obj)
    {
        this->reset();

        this->begin = obj.begin;
        this->end = obj.end;

        obj.begin = nullptr;
        obj.end = nullptr;

        return *this;
    }


    sentence::operator std::string()
    {
        if (this->begin != nullptr)
        {
            std::string retValue(this->begin, this->size());
            return retValue;
        }

        return std::string();
    }
    

    sentence& sentence::operator+=(char c)
    {
        const std::uint64_t size = this->size() + 1;
        char* data = nullptr;

        if (size == 1) { data = new char; }
        else if (size > 1) { data = new char[size]; }

        strncpy(data, this->begin, (size - 1));

        this->reset();

        data[size - 1] = c;

        this->begin = data;
        this->end = (this->begin + size - 1);

        return *this;
    }


    sentence& sentence::operator+=(const sentence& obj)
    {
        if (obj.begin != nullptr)
        {
            const std::uint64_t size = this->size() + obj.size();

            if (size > 0)
            {
                char* data = nullptr;

                if (size == 1) { data = new char; }
                else if (size > 1) { data = new char[size]; }

                strncpy(data, this->begin, this->size());
                if (obj.size() > 0) { strncpy((data + this->size()), obj.begin, obj.size()); }

                this->reset();

                this->begin = data;
                this->end = (this->begin + size - 1);
            }
        }

        return *this;
    }


    sentence operator+(const sentence& obj, const sentence& obj2)
    {
        sentence retValue = obj;
        retValue += obj2;

        return retValue;
    }


    sentence& sentence::getInput(const char to)
    {
        this->reset();

        const std::uint8_t increaseSize = sizeof(int*);
        std::uint64_t inputBytes = 0;

        this->begin = new char[increaseSize];
        this->end = (this->begin + increaseSize - 1);
        char* position = this->begin;

        while (true)
        {
            char inputByte = std::getchar();
            if (inputByte == to || inputByte == EOF) { break; }

            *position = inputByte;
            inputBytes += 1;

            if (inputBytes == increaseSize)
            {
                const std::uint64_t oldSize = this->size();

                char* beginTemp = new char[oldSize + increaseSize];
                strncpy(beginTemp, this->begin, oldSize);
                delete [] this->begin;

                this->begin = beginTemp;
                this->end = (this->begin + oldSize + increaseSize - 1);

                position = (this->begin + inputBytes - 1);
            }

            position += 1;
        }


        if (inputBytes < this->size())
        {
            if (inputBytes == 0)
            {
                delete [] begin;
                this->begin = nullptr;
                this->end = nullptr;
            }

            else if (inputBytes > 0)
            {
                char* beginTemp = nullptr;

                if (inputBytes == 1) { beginTemp = new char; }
                else if (inputBytes > 1) { beginTemp = new char[inputBytes]; }

                strncpy(beginTemp, this->begin, inputBytes);
                delete [] this->begin;

                this->begin = beginTemp;
                this->end = (this->begin + inputBytes - 1);
            }
        }

        return *this;
    }


    sentence& sentence::getInput(const std::uint64_t& size)
    {
        this->reset();

        std::uint64_t inputBytes = 0;

        if (size == 1) { this->begin = new char; }
        else if (size > 1) { this->begin = new char[size]; }

        this->end = (this->begin + size - 1);
        char* position = this->begin;

        while (true)
        {
            char inputByte = std::getchar();
            inputBytes += 1;

            if (inputBytes == size && inputByte != EOF && inputByte != '\n') { *position = inputByte; break; }
            else if (inputBytes == size || inputByte == '\n' || inputByte == EOF) { inputBytes -= 1; break; }

            *position = inputByte;
            position += 1;
        }


        if (inputBytes < size)
        {            
            if (inputBytes == 0)
            {
                if (size == 1) { delete begin; } 
                else if (size > 1) { delete [] begin; }

                this->begin = nullptr;
                this->end = nullptr;
            }

            else if (inputBytes > 0)
            {
                char* beginTemp = nullptr;

                if (inputBytes == 1) { beginTemp = new char; }
                else if (inputBytes > 1) { beginTemp = new char[inputBytes]; }

                strncpy(beginTemp, this->begin, inputBytes);
                
                if (size == 1) { delete begin; } 
                else if (size > 1) { delete [] begin; }

                this->begin = beginTemp;
                this->end = (this->begin + inputBytes - 1);
            }
        }

        return *this;
    }


    bool sentence::equal(const sentence& obj) const
    {
        if (this->size() != obj.size())
        {
            return false;
        }

        for (std::uint64_t i = 0; i < obj.size(); ++i)
        {
            if (this->begin[i] != obj.begin[i])
            {
                return false;
            }
        }

        return true;
    }


    void sentence::insert(const std::uint64_t& position, char c)
    {
        if (this->size() > 0 && position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        const std::uint64_t size = this->size() + 1;
        char* data = nullptr;

        if (size == 1) { data = new char; }
        else if (size > 1) { data = new char[size]; }

        for (std::uint64_t i = 0; i < size; ++i)
        {
            if (i < position)
            {
                data[i] = this->begin[i];
            }

            else if(i > position)
            {
                data[i] = this->begin[i - 1];
            }
        }

        this->reset();

        data[position] = c;

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    void sentence::remove(const std::uint64_t& position)
    {
        if (this->begin == nullptr || position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        const std::uint64_t size = this->size() - 1;
        char* data = nullptr;

        if (size == 1) { data = new char; }
        else if (size > 1) { data = new char[size]; }

        for (std::uint64_t i = 0; i < size; ++i)
        {
            if (i < position)
            {
                data[i] = this->begin[i];
            }

            else if(i > position)
            {
                data[i - 1] = this->begin[i];
            }
        }

        this->reset();

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    void sentence::toUpper()
    {
        if (this->begin != nullptr)
        {
            for (std::uint64_t i = 0; i < this->size(); ++i)
            {
                if (isalpha(this->begin[i]) == 0 && islower(this->begin[i]) == 0)
                {
                    this->begin[i] = toupper(this->begin[i]);
                }
            }
        }
    }


    void sentence::toLower()
    {
        if (this->begin != nullptr)
        {
            for (std::uint64_t i = 0; i < this->size(); ++i)
            {
                if (isalpha(this->begin[i]) == 0 && isupper(this->begin[i]) == 0)
                {
                    this->begin[i] = tolower(this->begin[i]);
                }
            }
        }
    }


    std::int64_t sentence::find(const char c, std::uint64_t* lastPosition) const
    {
        char* beginTemp = this->begin;
        bool is = false;
        const std::uint64_t size = this->size();
        std::int64_t position = 0;

        while (is == false || position == std::numeric_limits<std::int64_t>::max())
        {
            if (*beginTemp == c) { is = true; }
            else 
            {  
                position += 1;
                if (position == size) { break; }
                beginTemp += 1;
            }
        }

        if (lastPosition != nullptr && is == false && position == std::numeric_limits<std::int64_t>::max()) { *lastPosition = position; }
        if (is == false) { position = -1; }

        return position;
    }


    bool sentence::contains(sentence str)
    {
        if (this->begin == nullptr || str.begin == nullptr) 
        { 
            return false;
        }

        const std::uint64_t size = this->size();
        std::uint64_t word = 0;

        for (std::uint64_t i = 0; i < size; i++)
        {
            if (this->begin[i] == ' ' || i == (size - 1))
            {
                bool is = true;
                std::uint64_t temp = word;

                for (std::uint64_t k = 0; k < str.size(); k++)
                {
                    if (this->begin[temp] != str.begin[k])
                    {
                        is = false;
                        break;
                    }

                    temp += 1;
                }

                if (is == true) { return true; }

                word = i + 1;
            }
        }

        return false;
    }


    bool sentence::contains(sentence str, const size_t& at)
    {
        if (this->begin == nullptr || str.begin == nullptr) 
        { 
            return false;
        }

        const std::uint64_t size = this->size();
        std::uint64_t word = 0;
        std::uint64_t counter = 0;

        for (std::uint64_t i = 0; i < size; i++)
        {
            if (this->begin[i] == ' ' || i == (size - 1))
            {
                counter += 1;
                word = i + 1;
            }

            if (counter == at)
            {
                bool is = true;
                std::uint64_t temp = word;

                for (std::uint64_t k = 0; k < str.size(); k++)
                {
                    if (this->begin[temp] != str.begin[k])
                    {
                        is = false;
                        break;
                    }

                    temp += 1;
                }

                if (is == true) { return true; }

                word = i + 1;
            }
        }

        return false;
    }


    void sentence::reverse()
    {
        if (this->begin != nullptr && this->end > this->begin)
        {
            std::uint64_t halfSize = this->size() / 2;
            std::uint64_t i = 0;
            char* beginTemp = this->begin;
            char* endTemp = this->end;

            while (i != halfSize)
            {
                char temp = *beginTemp;
                *beginTemp = *endTemp;
                *endTemp = temp;

                beginTemp += 1;
                endTemp -= 1;
                i += 1;
            }
        }
    }


    void sentence::reverse(const std::uint64_t& from)
    {
        if (from > this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(from) + ")");
        }

        this->begin += from;
        reverse();
        this->begin -= from;
    }


    bool sentence::isNumber()
    {
        if (this->begin != nullptr)
        {
            if (equal(sentence(".")) == true || equal(sentence("-")) == true) 
            { 
                return false; 
            }

            bool tecka = false;
            bool minus = false;

            for (std::uint64_t i = 0; i < this->size(); ++i)
            {
                if (i == 0 && this->begin[0]  == '-')
                {
                    minus = true;
                }

                else if (isdigit(this->begin[i]) == 0)
                {
                    if (i == 0) { return false; }
                    else if (i == 1 && minus == true) { return false; }

                    if (i > 1 && this->begin[i] == '.' && tecka == false) { tecka = true; }
                    else { return false; }
                }
            }

            return true;
        }
        
        return false;
    }


    bool sentence::isDouble()
    {
        if (this->begin != nullptr)
        {
            std::uint64_t i = 0;

            if (this->begin[0] == '-') { i = 1; }

            for (i = i; i < this->size(); ++i)
            {
                if (this->begin[i] == '.')
                {
                    for (std::uint64_t k = (i + 1); k < this->size(); ++k)
                    {
                        if (isdigit(this->begin[k]) == 0) 
                        { 
                            return false;
                        }
                    }

                    return true;
                }
            }

            return false;
        }
        
        return false;
    }


    bool sentence::empty() const
    {
        return (this->begin == nullptr);
    }


    long double sentence::toNumber()
    {
        if (this->begin == nullptr || isNumber() == false) 
        { 
            return 0; 
        }

        long double vys = 0;
        std::int64_t on = -1;

        for (std::uint64_t i = 0; i < this->size(); ++i)
        {
            if (this->begin[i] == '.' || this->begin[i] == ',')
            {
                on = i - 1;
            }

            else if (i == this->size() - 1 && on == -1)
            {
                on = this->size() - 1;

                for (std::uint64_t i = 0; i < this->size(); ++i)
                {
                    short int temp = this->begin[i] - '0';
                    vys += temp * pow(10, on);
                    on -= 1;
                }

                return vys;
            }
        }


        std::uint64_t delitel = 10;
        for (std::uint64_t i = (on + 2); i < this->size(); ++i)
        {
            short int temp = this->begin[i] - '0';
            vys += (long double) temp / (long double) delitel;
            delitel *= 10;
        }


        delitel = on;
        for (std::uint64_t i = 0; i <= on; ++i)
        {
            short int temp = this->begin[i] - '0';
            vys += temp * pow(10, delitel);
            delitel -= 1;
        }

        return vys;
    }


    sentence& operator<<(sentence& obj, const sentence& str)
    {
        obj += str;
        return obj;
    }


    std::ostream& sentence::print(std::ostream& os) const
    {
        os.flush();

        for (std::uint64_t i = 0; i < this->size(); ++i)
        {
            os << this->begin[i];
        }

        return os;
    }


    std::ostream& operator<<(std::ostream& os, const sentence& obj)
    {
        return obj.print(os);
    }
}



#endif  // SENTENCE_HPP