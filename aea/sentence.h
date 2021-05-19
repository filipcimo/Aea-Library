#ifndef SENTENCE_H
#define SENTENCE_H
#include "BasicContainer.h"
#include <math.h>



namespace aea
{
    class sentence : public BasicContainer<char>
    {
        public:
            sentence() = default;
            sentence(char c);
            sentence(const char* str);
            sentence(const std::string& str);
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

            sentence& operator+=(char c);
            sentence& operator+=(const sentence& obj);
            friend sentence operator+(const sentence& obj, const sentence& obj2);

            friend bool operator==(const sentence& it, const sentence& it2);
            friend bool operator!=(const sentence& it, const sentence& it2);

            void insert(const std::uint64_t& position, char c);
            void remove(const std::uint64_t& position);
            void toUpper();
            void toLower();
	    bool contains(sentence str);
	    bool contains(sentence str, const size_t& at);
            long double ston();
            bool isNumber();
            bool isDouble();


            friend std::ostream& operator<<(std::ostream& os, const sentence& obj);

        protected:
            std::ostream& print(std::ostream& os) const;
    };



    sentence::sentence(char c)
    {
        this->begin = new char(c);
        this->end = (this->begin + sizeof(char));
    }


    sentence::sentence(const char* str)
    {
        const std::uint64_t size = strlen(str); 

        this->begin = new char[size];
        this->end = (this->begin + size);

        strncpy(this->begin, str, size);
    }


    sentence::sentence(const std::string& str)
    {
        const std::uint64_t size = str.size(); 

        this->begin = new char[size];
        this->end = (this->begin + size);

        strncpy(this->begin, str.data(), size);
    }


    sentence::sentence(const std::initializer_list<char>& list) : BasicContainer<char>(list)
    {

    }

    
    sentence::sentence(const sentence& obj) : BasicContainer<char>(obj)
    {

    }


    sentence::sentence(sentence&& obj) : BasicContainer<char>(obj)
    {
        
    }


    sentence& sentence::operator=(char c)
    {
        this->reset();

        this->begin = new char(c);
        this->end = (this->begin + sizeof(char));

        return *this;
    }


    sentence& sentence::operator=(const char* str)
    {
        this->reset();

        if (str != nullptr)
        {
            const std::uint64_t size = strlen(str);

            this->begin = new char[size];
            this->end = (this->begin + size);

            strncpy(this->begin, str, size);
        }

        return *this;
    }


    sentence& sentence::operator=(const std::string& str)
    {
        this->reset();

        if (str.empty() == false)
        {
            const std::uint64_t size = str.size();

            this->begin = new char[size];
            this->end = (this->begin + size);

            strncpy(this->begin, str.data(), size);
        }

        return *this;
    }


    sentence& sentence::operator=(const std::initializer_list<char>& list)
    {
        this->reset();

        const std::uint64_t size = list.size();

        this->begin = new char[size];
        this->end = (this->begin + size);

        for (std::uint64_t i = 0; i < size; ++i)
        {
            this->begin[i] = *list.begin() + i;
        }

        return *this;
    }

    
    sentence& sentence::operator=(const sentence& obj)
    {
        if (obj.begin != nullptr)
        {
            this->reset();

            const std::uint64_t size = obj.size();

            this->begin = new char[size];
            this->end = (this->begin + size);

            strncpy(this->begin, obj.begin, size);
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


    sentence& sentence::operator+=(char c)
    {
        const std::uint64_t size = (this->end - this->begin) + 1;

        char* data = new char[size];
        strncpy(data, this->begin, (size - 1));

        this->reset();

        data[size - 1] = c;

        this->begin = data;
        this->end = (this->begin + size);

        return *this;
    }


    sentence& sentence::operator+=(const sentence& obj)
    {
        if (obj.begin != nullptr)
        {
            const std::uint64_t size = (this->end - this->begin);

            char* data = new char[size];
            strncpy(data, this->begin, size);

            this->reset();

            strncpy((data + size), obj.begin, obj.size());

            this->begin = data;
            this->end = (this->begin + (size + obj.size()));
        }

        return *this;
    }


    sentence operator+(const sentence& obj, const sentence& obj2)
    {
        sentence retValue = obj;
        retValue += obj2;

        return retValue;
    }


    bool operator==(const sentence& obj, const sentence& obj2)
    {
        if (obj.size() != obj2.size())
        {
            return false;
        }

        for (std::uint64_t i = 0; i < obj.size(); ++i)
        {
            if (obj.at(i) != obj2.at(i))
            {
                return false;
            }
        }

        return true;
    }


    bool operator!=(const sentence& obj, const sentence& obj2)
    {
        return !(obj == obj2);
    }


    void sentence::insert(const std::uint64_t& position, char c)
    {
        if (this->begin == nullptr || position >= (this->end - this->begin))
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        const std::uint64_t size = (this->end - this->begin) + 1;
        char* data = new char[size];

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
        this->end = (this->begin + size);
    }


    void sentence::remove(const std::uint64_t& position)
    {
        if (this->begin == nullptr || position >= (this->end - this->begin))
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        const std::uint64_t size = (this->end - this->begin) - 1;
        char* data = new char[size];

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
        this->end = (this->begin + size);
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


    bool sentence::contains(sentence str)
    {
        if (this->begin == nullptr || str.begin == nullptr) 
        { 
            return false;
        }

        const std::uint64_t size = (this->end - this->begin);
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

        const std::uint64_t size = (this->end - this->begin);
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


    bool sentence::isNumber()
    {
        if (this->begin != nullptr)
        {
            if (*this == "." || *this == "-") 
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


    long double sentence::ston()
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


    std::ostream& sentence::print(std::ostream& os) const
    {
        os.flush();

        for (std::uint64_t i = 0; i < (this->end - this->begin); ++i)
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



#endif  // SENTENCE_H
