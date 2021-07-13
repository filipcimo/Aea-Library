#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "BasicContainer.hpp"
#include "ListNode.hpp"



namespace aea
{
    template<typename T>
    class Queue : public BasicContainer<ListNode<T>>
    {   
        public:
            Queue() = default;
            Queue(const Queue<T>& obj);
            Queue(Queue<T>&& obj);
            explicit Queue(const std::uint64_t& size);
            Queue(const std::initializer_list<ListNode<T>>& list);
            virtual ~Queue();

            Queue<T>& operator=(const Queue<T>& obj);
            Queue<T>& operator=(Queue<T>&& obj);
            Queue<T>& operator=(const std::initializer_list<ListNode<T>>& list);

            virtual void reset();
            virtual ListNode<T>* get() const;
            virtual ListNode<T>& at(const std::uint64_t& position) const;
            virtual ListNode<T>& operator[](const std::uint64_t& position) const;
            virtual std::uint64_t size() const;
            void add(const ListNode<T>& item);
            void add(ListNode<T>&& item);
            void add(const std::uint64_t& position, const ListNode<T>& item);
            void add(const std::uint64_t& position, ListNode<T>&& item);
            void remove();
            void remove(const std::uint64_t& position);

        
        protected:
            virtual std::ostream& print(std::ostream& os) const;
    };


    
    template<typename T>
    Queue<T>::~Queue()
    {
        ListNode<T>* temp = this->begin;

        while (temp != nullptr)
        {
            temp = this->begin->nextNode();
            delete this->begin;
            this->begin = temp;
        }
    }


    template<typename T>
    Queue<T>::Queue(const Queue<T>& obj) : BasicContainer<ListNode<T>>()
    {
        ListNode<T>* temp = obj.begin;

        while (temp != nullptr)
        {
            this->add(*temp);
            temp = temp->nextNode();
        }
    }


    template<typename T>
    Queue<T>::Queue(Queue<T>&& obj)
    {
        this->begin = obj.begin;
        this->end = obj.end;

        obj.begin = nullptr;
        obj.end = nullptr;
    }


    template<typename T>
    Queue<T>::Queue(const std::uint64_t& size)
    {
        for (std::uint64_t i = 0; i < size; ++i)
        {
            this->add(std::move(ListNode<T>()));
        }
    }


    template<typename T>
    Queue<T>::Queue(const std::initializer_list<ListNode<T>>& list)
    {
        for (const ListNode<T>& node : list)
        {
            this->add(node);
        }
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(const Queue<T>& obj)
    {
        this->reset();

        ListNode<T>* temp = obj.begin;

        while (temp != nullptr)
        {
            this->add(*temp);
            temp = temp->nextNode();
        }

        return *this;
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(Queue<T>&& obj)
    {
        this->reset();

        this->begin = obj.begin;
        this->end = obj.end;

        obj.begin = nullptr;
        obj.end = nullptr;

        return *this;
    }


    template<typename T>
    Queue<T>& Queue<T>::operator=(const std::initializer_list<ListNode<T>>& list)
    {
        this->reset();

        for (const ListNode<T>& node : list)
        {
            this->add(node);
        }

        return *this;
    }


    template<typename T>
    void Queue<T>::reset()
    {
        ListNode<T>* temp = this->begin;

        while (this->begin != nullptr)
        {
            temp = this->begin->nextNode();
            delete this->begin;
            this->begin = temp;
        }

        this->end = nullptr;
    }


    template<typename T>
    std::uint64_t Queue<T>::size() const
    {
        ListNode<T>* temp = this->begin;
        std::uint64_t i = 0;

        while (temp != nullptr)
        {
            temp = temp->nextNode();
            i += 1;
        }

        return i;
    }


    template<typename T>
    ListNode<T>* Queue<T>::get() const
    {
        return this->begin;
    }


    template<typename T>
    ListNode<T>& Queue<T>::at(const std::uint64_t& position) const
    {
        if (this->begin == nullptr || position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }


        ListNode<T>* temp = this->begin;

        for (std::uint64_t i = 0; i < position; ++i)
        {
            temp = temp->nextNode();
        }

        return *temp;
    }


    template<typename T>
    ListNode<T>& Queue<T>::operator[](const std::uint64_t& position) const
    {
        if (this->begin == nullptr || position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }
        

        ListNode<T>* temp = this->begin;

        for (std::uint64_t i = 0; i < position; ++i)
        {
            temp = temp->nextNode();
        }

        return *temp;
    }


    template<typename T>
    void Queue<T>::add(const ListNode<T>& item)
    {
        ListNode<T>* node = new ListNode<T>(item);

        if (this->begin == nullptr)
        {
            this->begin = node;
            this->end = this->begin;
        }

        else
        {
            this->end->nextNode(node);
            this->end = node;
        }
    }


    template<typename T>
    void Queue<T>::add(ListNode<T>&& item)
    {
        ListNode<T>* node = new ListNode<T>(std::move(item));

        if (this->begin == nullptr)
        {
            this->begin = node;
            this->end = this->begin;
        }

        else
        {
            this->end->nextNode(node);
            this->end = node;
        }
    }


    template<typename T>
    void Queue<T>::add(const std::uint64_t& position, const ListNode<T>& item)
    {
        if (position > this->size() && position != 0)
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }


        ListNode<T>* node = new ListNode<T>(item);

        if (position == 0)
        {
            this->begin = node;
            this->end = this->begin;
        }

        else if (position > 0)
        {
            ListNode<T>* temp = this->begin;

            for (std::uint64_t i = 0; i < (position - 1); ++i)
            {
                temp = temp->nextNode();
            }

            ListNode<T>* nextTemp = temp->nextNode(); 
            temp->nextNode(node);
            temp->nextNode()->nextNode(nextTemp);
        }
    }


    template<typename T>
    void Queue<T>::add(const std::uint64_t& position, ListNode<T>&& item)
    {
        if (position > this->size() && position != 0)
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }


        ListNode<T>* node = new ListNode<T>(std::move(item));

        if (position == 0)
        {
            this->begin = node;
            this->end = this->begin;
        }

        else if (position > 0)
        {
            ListNode<T>* temp = this->begin;

            for (std::uint64_t i = 0; i < (position - 1); ++i)
            {
                temp = temp->nextNode();
            }

            ListNode<T>* nextTemp = temp->nextNode(); 
            temp->nextNode(node);
            temp->nextNode()->nextNode(nextTemp);
        }
    }


    template<typename T>
    void Queue<T>::remove()
    {
        if (this->begin != nullptr)
        {
            if (this->begin == this->end)
            {
                delete this->begin;

                this->begin = nullptr;
                this->end = nullptr;
            }

            else
            {
                ListNode<T>* temp = this->begin->nextNode();
                delete this->begin;

                this->begin = temp;
                if (this->begin->nextNode() == nullptr) { this->end = this->begin; }
            }
        }
    }


    template<typename T>
    void Queue<T>::remove(const std::uint64_t& position)
    {
        if (this->begin == nullptr || position >= this->size())
        {
            throw std::out_of_range("Index out of range (Index: " + std::to_string(position) + ")");
        }


        if (position == 0)
        {
            ListNode<T>* temp = this->begin->nextNode();
            delete this->begin;
            this->begin = temp;
        }

        else if (position > 0)
        {
            ListNode<T>* temp = this->begin;

            for (std::uint64_t i = 0; i < (position - 1); ++i)
            {
                temp = temp->nextNode();
            }

            ListNode<T>* nextTemp = temp->nextNode()->nextNode(); 
            delete temp->nextNode();
            temp->nextNode(nextTemp);
        }


        if (this->begin == nullptr)
        {
            this->end = nullptr;
        }
    }


    template<typename T>
    std::ostream& Queue<T>::print(std::ostream& os) const
    {
        return os;
    }


    template<typename Type> 
    ListNode<Type>* iterate_front(ListNode<Type>** accesser, const Queue<Type>& obj, const ListNode<Type>* const start, const std::uint64_t& by = 1)
    {
        if (accesser == nullptr || accesser[0] == obj.last())
        {
            return nullptr;
        }

        else if (accesser[0] == nullptr)
        {
            accesser[0] = (ListNode<Type>*) start;
        }

        else
        {
            for (std::uint64_t i = 0; i < by; ++i)
            {
                accesser[0] = accesser[0]->nextNode();
            }
        }

        return accesser[0];
    }
}



#endif  // QUEUE_HPP