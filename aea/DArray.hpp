#ifndef DARRAY_HPP
#define DARRAY_HPP
#include "BasicContainer.hpp"



namespace aea
{
    template<typename T>
    class DArray : public BasicContainer<T>
    {
        public:
            DArray() = default;
            explicit DArray(const std::uint64_t& size);
            DArray(const std::initializer_list<T>& list);
            DArray(const T* const ptr, const std::uint64_t& size);
            DArray(const DArray<T>& obj);
            DArray(DArray<T>&& obj);
            virtual ~DArray() = default;

            DArray<T>& operator=(const DArray<T>& obj);
            DArray<T>& operator=(DArray<T>&& obj);

            void pushback(const T& item);
            void popback();
            void pushfront(const T& item);
            void popfront();
            void insert(const std::uint64_t& position, const T& item);
            void remove(const std::uint64_t& position);
            void resize(const std::uint64_t& size);
            template<typename Type> friend std::ostream& operator<<(std::ostream& os, const DArray<Type>& obj);


        protected:
            virtual std::ostream& print(std::ostream& os) const;
    };



    template<typename T>
    DArray<T>::DArray(const std::uint64_t& size)
    {
        this->begin = new T[size];
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    DArray<T>::DArray(const std::initializer_list<T>& list) : BasicContainer<T>(list)
    {

    }


    template<typename T>
    DArray<T>::DArray(const DArray<T>& obj) : BasicContainer<T>(obj)
    {
        
    }


    template<typename T>
    DArray<T>::DArray(DArray<T>&& obj) : BasicContainer<T>(std::move(obj))
    {
        
    }


    template<typename T>
    DArray<T>::DArray(const T* const ptr, const std::uint64_t& size)
    {
        if (ptr != nullptr && size > 0)
        {
            this->begin = new T[size];
            this->end = (this->begin + size - 1);

            aea::arrcpy(this->begin, ptr, size);
        }
    }


    template<typename T>
    DArray<T>& DArray<T>::operator=(const DArray<T>& obj)
    {   
        if (obj.begin != nullptr)
        {
            this->reset();

            const std::uint64_t size = obj.size();

            this->begin = new T[size];
            this->end = (this->begin + size - 1);

            aea::arrcpy(this->begin, obj.begin, size);
        }

        return *this;
    }


    template<typename T>
    DArray<T>& DArray<T>::operator=(DArray<T>&& obj)
    {
        this->reset();

        this->begin = obj.begin;
        this->end = obj.end;

        obj.begin = nullptr;
        obj.end = nullptr;

        return *this;
    }


    template<typename T>
    std::ostream& DArray<T>::print(std::ostream& os) const
    {
        os.flush();

        for (std::uint64_t i = 0; i < this->size(); ++i)
        {
            os << this->begin[i] << ' ';
        }

        return os;
    }


    template<typename Type>
    std::ostream& operator<<(std::ostream& os, const DArray<Type>& obj)
    {
        return obj.print(os);
    }


    template<typename T>
    void DArray<T>::resize(const std::uint64_t& size)
    {
        this->reset();

        if (size == 0) { return; }

        this->begin = new T[size];
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DArray<T>::pushback(const T& item)
    {
        const std::uint64_t size = this->size() + 1;

        T* data = new T[size];
        aea::arrcpy(data, this->begin, (size - 1));

        this->reset();

        data[size - 1] = item;

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DArray<T>::popback()
    {
        const std::uint64_t size = this->size() - 1;

        T* data = new T[size];
        aea::arrcpy(data, this->begin, size);

        this->reset();

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DArray<T>::pushfront(const T& item)
    {
        const std::uint64_t size = this->size() + 1;

        T* data = new T[size];
        aea::arrcpy((data + 1), this->begin, (size - 1));

        this->reset();

        data[0] = item;

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DArray<T>::popfront()
    {
        const std::uint64_t size = this->size() - 1;

        T* data = new T[size];
        aea::arrcpy(data, (this->begin + 1), size);

        this->reset();

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DArray<T>::insert(const std::uint64_t& position, const T& item)
    {
        if (this->begin == nullptr || position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        const std::uint64_t size = this->size() + 1;
        T* data = new T[size];

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

        data[position] = item;

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DArray<T>::remove(const std::uint64_t& position)
    {
        if (this->begin == nullptr || position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        const std::uint64_t size = this->size() - 1;
        T* data = new T[size];

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
}



#endif  // DARRAY_H
