#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "List.hpp"
#include "Pointer.hpp"



namespace aea
{
    template<typename T>
    class Queue
    {
        public:
            Queue();
            Queue(const Queue<T>& obj);
            Queue(Queue<T>&& obj);
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

            template<typename Type> friend std::ostream& operator<<(std::ostream& os, const Queue<Type>& obj);


        private:
            aea::Pointer<aea::List<T>> data;
    };


    template<typename T>
    Queue<T>::Queue()
        : data(std::move(aea::Pointer<aea::List<T>>(new aea::List<T>())))
    {

    }


    template<typename T>
    Queue<T>::Queue(const Queue<T>& obj)
        : data(std::move(aea::Pointer<aea::List<T>>(new aea::List<T>())))
    {
        *data.get() = *obj.data.get();
    }


    template<typename T>
    Queue<T>::Queue(const std::initializer_list<T>& list)
        : data(std::move(aea::Pointer<aea::List<T>>(new aea::List<T>())))
    {
        for (const T& item : list) 
        {
            data.get()->add(item);
        }
    }


    template<typename T>
    Queue<T>::Queue(const std::initializer_list<ListNode<T>>& list)
        : data(std::move(aea::Pointer<aea::List<T>>(new aea::List<T>(list))))
    {

    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(const Queue<T>& obj)
    {
        *data.get() = *obj.data.get();
        return *this;
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(Queue<T>&& obj)
    {
        *data.get() = std::move(*obj.data.get());
        return *this;
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(const std::initializer_list<T>& list)
    {
        this->reset();

        for (const T& item : list) 
        {
            data.get()->add(item);
        }

        return *this;
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(const std::initializer_list<ListNode<T>>& list)
    {
        this->reset();

        for (const T& item : list) 
        {
            data.get()->add(item);
        }

        return *this;
    }


    template<typename T>
    void Queue<T>::reset()
    {
        data.get()->reset();
    }


    template<typename T>
    aea::ListNode<T>* Queue<T>::get() const
    {
        return data.get()->get();
    }


    template<typename T>
    T& Queue<T>::at(const std::uint64_t& position) const
    {
        return *data.get()->at(position).dataNode();
    }


    template<typename T>
    T& Queue<T>::operator[](const std::uint64_t& position) const
    {
        return *data.get()->at(position).dataNode();
    }


    template<typename T>
    std::uint64_t Queue<T>::size() const
    {
        return data.get()->size();
    }


    template<typename T>
    void Queue<T>::add(const T& item)
    {
        data.get()->add(item);
    }


    template<typename T>
    void Queue<T>::add(const std::uint64_t& position, const T& item)
    {
        data.get()->add(position, item);
    }


    template<typename T>
    void Queue<T>::remove()
    {
        if (data.get()->size() != 0)
        {
            data.get()->remove(0);
        }
    }


    template<typename T>
    void Queue<T>::remove(const std::uint64_t& position)
    {
        data.get()->remove(position);
    }


    template<typename Type> 
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
    }
}



#endif  // QUEUE_HPP