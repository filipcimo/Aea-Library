#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include "BasicContainer.hpp"



namespace aea
{
    template<typename T>
    class DynamicArray : public BasicContainer<T>
    {
        public:
            DynamicArray() = default;
            explicit DynamicArray(const std::uint64_t& size);
            DynamicArray(const std::initializer_list<T>& list);
            DynamicArray(const T* const ptr, const std::uint64_t& size);
            DynamicArray(const DynamicArray<T>& obj);
            DynamicArray(DynamicArray<T>&& obj);
            virtual ~DynamicArray() = default;

            DynamicArray<T>& operator=(const DynamicArray<T>& obj);
            DynamicArray<T>& operator=(DynamicArray<T>&& obj);

            void pushback(const T& item);
            void pushback(T&& item);
            void popback();
            void pushfront(const T& item);
            void pushfront(T&& item);
            void popfront();
            void insert(const std::uint64_t& position, const T& item);
            void insert(const std::uint64_t& position, T&& item);
            void remove(const std::uint64_t& position);
            void resize(const std::uint64_t& size);


        protected:
            virtual std::ostream& print(std::ostream& os) const override;
    };



    template<typename T>
    DynamicArray<T>::DynamicArray(const std::uint64_t& size)
    {   
        if (size > 0)
        {
            this->begin = this->getAllocatedMemory(size);
            this->end = (this->begin + size - 1);
        }
    }


    template<typename T>
    DynamicArray<T>::DynamicArray(const std::initializer_list<T>& list) : BasicContainer<T>(list)
    {

    }


    template<typename T>
    DynamicArray<T>::DynamicArray(const DynamicArray<T>& obj) : BasicContainer<T>(obj)
    {
        
    }


    template<typename T>
    DynamicArray<T>::DynamicArray(DynamicArray<T>&& obj) : BasicContainer<T>(std::move(obj))
    {
        
    }


    template<typename T>
    DynamicArray<T>::DynamicArray(const T* const ptr, const std::uint64_t& size)
    {
        if (ptr != nullptr && size > 0)
        {
            this->begin = this->getAllocatedMemory(size);
            this->end = (this->begin + size - 1);

            aea::arrcopy(this->begin, ptr, size);
        }
    }


    template<typename T>
    DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& obj)
    {   
        if (obj.begin != nullptr)
        {
            this->reset();

            const std::uint64_t size = obj.size();

            if (size > 0)
            {
                this->begin = this->getAllocatedMemory(size);
                this->end = (this->begin + size - 1);

                aea::arrcopy(this->begin, obj.begin, size);
            }
        }

        return *this;
    }


    template<typename T>
    DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& obj)
    {
        this->reset();

        this->begin = obj.begin;
        this->end = obj.end;

        obj.begin = nullptr;
        obj.end = nullptr;

        return *this;
    }


    template<typename T>
    std::ostream& DynamicArray<T>::print(std::ostream& os) const
    {
        return os;
    }


    template<typename T>
    void DynamicArray<T>::resize(const std::uint64_t& size)
    {
        if (size == 0) 
        { 
            this->reset();
        }

        else if (size == this->size())
        {
            return;
        }

        else 
        {
            T* newData = this->getAllocatedMemory(size);

            if (size > this->size()) { aea::arrmove(newData, this->begin, this->size()); }
            else if (size < this->size()) { aea::arrmove(newData, this->begin, size); }

            this->reset();

            this->begin = newData;
            this->end = (this->begin + size - 1);
        }
    }


    template<typename T>
    void DynamicArray<T>::pushback(const T& item)
    {
        const std::uint64_t size = this->size() + 1;
        T* data = this->getAllocatedMemory(size);

        aea::arrmove(data, this->begin, (size - 1));
        this->reset();

        data[size - 1] = item;

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DynamicArray<T>::pushback(T&& item)
    {
        const std::uint64_t size = this->size() + 1;
        T* data = this->getAllocatedMemory(size);

        aea::arrmove(data, this->begin, (size - 1));
        this->reset();

        data[size - 1] = std::move(item);

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DynamicArray<T>::popback()
    {
        if (this->begin != nullptr)
        {
            const std::uint64_t size = this->size() - 1;
            T* data = this->getAllocatedMemory(size);

            aea::arrmove(data, this->begin, size);
            this->reset();

            if (size > 0)
            {
                this->begin = data;
                this->end = (this->begin + size - 1);
            }
        }
    }


    template<typename T>
    void DynamicArray<T>::pushfront(const T& item)
    {
        const std::uint64_t size = this->size() + 1;
        T* data = this->getAllocatedMemory(size);

        aea::arrmove((data + 1), this->begin, (size - 1));
        this->reset();

        data[0] = item;

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DynamicArray<T>::pushfront(T&& item)
    {
        const std::uint64_t size = this->size() + 1;
        T* data = this->getAllocatedMemory(size);

        aea::arrmove((data + 1), this->begin, (size - 1));
        this->reset();

        data[0] = std::move(item);

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DynamicArray<T>::popfront()
    {
        if (this->begin != nullptr)
        {
            const std::uint64_t size = this->size() - 1;
            T* data = this->getAllocatedMemory(size);

            aea::arrmove(data, (this->begin + 1), size);
            this->reset();

            if (size > 0)
            {
                this->begin = data;
                this->end = (this->begin + size - 1);
            }
        }
    }


    template<typename T>
    void DynamicArray<T>::insert(const std::uint64_t& position, const T& item)
    {
        if (this->begin == nullptr || position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        const std::uint64_t size = this->size() + 1;
        T* data = this->getAllocatedMemory(size);

        for (std::uint64_t i = 0; i < size; ++i)
        {
            if (i < position)
            {
                data[i] = std::move(this->begin[i]);
            }

            else if(i > position)
            {
                data[i] = std::move(this->begin[i - 1]);
            }
        }

        this->reset();

        data[position] = item;

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DynamicArray<T>::insert(const std::uint64_t& position, T&& item)
    {
        if (this->begin == nullptr || position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        const std::uint64_t size = this->size() + 1;
        T* data = this->getAllocatedMemory(size);

        for (std::uint64_t i = 0; i < size; ++i)
        {
            if (i < position)
            {
                data[i] = std::move(this->begin[i]);
            }

            else if(i > position)
            {
                data[i] = std::move(this->begin[i - 1]);
            }
        }

        this->reset();

        data[position] = std::move(item);

        this->begin = data;
        this->end = (this->begin + size - 1);
    }


    template<typename T>
    void DynamicArray<T>::remove(const std::uint64_t& position)
    {
        if (this->begin == nullptr || position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }

        const std::uint64_t size = this->size() - 1;
        T* data = this->getAllocatedMemory(size);

        for (std::uint64_t i = 0; i < size; ++i)
        {
            if (i < position)
            {
                data[i] = std::move(this->begin[i]);
            }

            else if(i > position)
            {
                data[i - 1] = std::move(this->begin[i]);
            }
        }

        this->reset();

        if (size > 0)
        {
            this->begin = data;
            this->end = (this->begin + size - 1);
        }
    }
}



#endif  // DYNAMIC_ARRAY_HPP