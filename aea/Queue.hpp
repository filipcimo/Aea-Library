#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "List.hpp"



namespace aea
{
    template<typename T>
    class Queue
    {
        public:
            Queue() = default;
            Queue(const Queue<T>& obj);
            Queue(Queue<T>&& obj);
            Queue(const std::uint64_t& size);
            Queue(const std::initializer_list<T>& list);
            Queue(const std::initializer_list<ListNode<T>>& list);
            ~Queue() = default;

            Queue<T>& operator=(const Queue<T>& obj);
            Queue<T>& operator=(Queue<T>&& obj);
            Queue<T>& operator=(const std::initializer_list<T>& list);
            Queue<T>& operator=(const std::initializer_list<ListNode<T>>& list);


            void reset();
            aea::ListNode<T>* get() const;
            T& at(const std::uint64_t& position) const;
            T& operator[](const std::uint64_t& position) const;
            std::uint64_t size() const;
            void add(const T& item);
            void add(const std::uint64_t& position, const T& item);
            void remove();
            void remove(const std::uint64_t& position);

            //template<typename Type> friend std::ostream& operator<<(std::ostream& os, const Queue<Type>& obj);


        private:
            aea::List<T> data;
    };


    template<typename T>
    Queue<T>::Queue(const Queue<T>& obj)
    {
        data = obj.data;
    }


    template<typename T>
    Queue<T>::Queue(Queue<T>&& obj)
    {
        data = std::move(obj.data);
    }


    template<typename T>
    Queue<T>::Queue(const std::uint64_t& size)
    {
        data = std::move(aea::List<T>(size));
    }


    template<typename T>
    Queue<T>::Queue(const std::initializer_list<T>& list)
    {
        for (const T& item : list) 
        {
            data.add(item);
        }
    }


    template<typename T>
    Queue<T>::Queue(const std::initializer_list<ListNode<T>>& list)
    {
        data = std::move(aea::List<T>(list));
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(const Queue<T>& obj)
    {
        data = obj.data;
        return *this;
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(Queue<T>&& obj)
    {
        data = std::move(obj.data);
        return *this;
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(const std::initializer_list<T>& list)
    {
        this->reset();

        for (const T& item : list) 
        {
            data.add(item);
        }

        return *this;
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(const std::initializer_list<ListNode<T>>& list)
    {
        this->reset();

        for (const ListNode<T>& item : list) 
        {
            data.add(item);
        }

        return *this;
    }


    template<typename T>
    void Queue<T>::reset()
    {
        data.reset();
    }


    template<typename T>
    aea::ListNode<T>* Queue<T>::get() const
    {
        return data.get();
    }


    template<typename T>
    T& Queue<T>::at(const std::uint64_t& position) const
    {
        return *data.at(position).dataNode();
    }


    template<typename T>
    T& Queue<T>::operator[](const std::uint64_t& position) const
    {
        return *data.at(position).dataNode();
    }


    template<typename T>
    std::uint64_t Queue<T>::size() const
    {
        return data.size();
    }


    template<typename T>
    void Queue<T>::add(const T& item)
    {
        data.add(item);
    }


    template<typename T>
    void Queue<T>::add(const std::uint64_t& position, const T& item)
    {
        data.add(position, item);
    }


    template<typename T>
    void Queue<T>::remove()
    {
        if (data.size() != 0)
        {
            data.remove(0);
        }
    }


    template<typename T>
    void Queue<T>::remove(const std::uint64_t& position)
    {
        data.remove(position);
    }


    /*template<typename Type> 
    std::ostream& operator<<(std::ostream& os, const Queue<Type>& obj)
    {
        if (obj.size() != 0)
        {
            for (std::uint64_t i = 0; i < obj.size(); ++i)
            {
                os << obj.at(i) << ' ';
            }
        }

        else
        {
            os << '\0';
        }

        return os;
    }*/
}



#endif  // QUEUE_HPP