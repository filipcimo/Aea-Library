#ifndef LIST_HPP
#define LIST_HPP
#include "BasicContainer.hpp"
#include "ListNode.hpp"



namespace aea
{
    template<typename T>
    class List : public BasicContainer<ListNode<T>>
    {   
        public:
            List() = default;
            List(const List<T>& obj);
            List(List<T>&& obj);
            List(const std::initializer_list<ListNode<T>>& list);
            virtual ~List();

            List<T>& operator=(const List<T>& obj);
            List<T>& operator=(List<T>&& obj);

            virtual void reset();
            virtual ListNode<T>& at(const std::uint64_t& position) const;
            virtual ListNode<T>& operator[](const std::uint64_t& position) const;
            virtual std::uint64_t size() const;
            void add(const ListNode<T>& item);
            void add(const std::uint64_t& position, const ListNode<T>& item);
            void remove();
            void remove(const std::uint64_t& position);

            template<typename Type> friend std::ostream& operator<<(std::ostream& os, const List<Type>& obj);

        
        protected:
            virtual std::ostream& print(std::ostream& os) const;
    };


    
    template<typename T>
    List<T>::~List()
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
    List<T>::List(const List<T>& obj) : BasicContainer<ListNode<T>>()
    {
        ListNode<T>* temp = obj.begin;

        while (temp != nullptr)
        {
            this->add(*temp);
            temp = temp->nextNode();
        }
    }


    template<typename T>
    List<T>::List(List<T>&& obj)
    {
        this->begin = obj.begin;
        this->end = obj.end;

        obj.begin = nullptr;
        obj.end = nullptr;
    }


    template<typename T>
    List<T>::List(const std::initializer_list<ListNode<T>>& list)
    {
        for (std::uint64_t i = 0; i < list.size(); ++i)
        {
            this->add(*(list.begin() + i));
        }
    }


    template<typename T>
    List<T>& List<T>::operator=(const List<T>& obj)
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
    List<T>& List<T>::operator=(List<T>&& obj)
    {
        this->reset();

        this->begin = obj.begin;
        this->end = obj.end;

        obj.begin = nullptr;
        obj.end = nullptr;

        return *this;
    }


    template<typename T>
    void List<T>::reset()
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
    std::uint64_t List<T>::size() const
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
    ListNode<T>& List<T>::at(const std::uint64_t& position) const
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
    ListNode<T>& List<T>::operator[](const std::uint64_t& position) const
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
    void List<T>::add(const ListNode<T>& item)
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
    void List<T>::add(const std::uint64_t& position, const ListNode<T>& item)
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
    void List<T>::remove()
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
                ListNode<T>* temp = this->begin;

                while (temp->nextNode()->nextNode() != nullptr)
                {
                    temp = temp->nextNode();
                }

                delete temp->nextNode();

                temp->nextNode(nullptr);
                this->end = temp;
            }
        }
    }


    template<typename T>
    void List<T>::remove(const std::uint64_t& position)
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
    std::ostream& List<T>::print(std::ostream& os) const
    {
        ListNode<T>* temp = (ListNode<T>*) this->begin;

        while (temp != nullptr)
        {
            os << *temp->dataNode() << ' ';
            temp = temp->nextNode();
        }

        return os;
    }


    template<typename Type>
    std::ostream& operator<<(std::ostream& os, const List<Type>& obj)
    {
        return obj.print(os);
    }


    template<typename Type> 
    ListNode<Type>* iterate_front(ListNode<Type>** accesser, const List<Type>& obj, const ListNode<Type>* const start, const std::uint64_t& by = 1)
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



#endif  // LIST_HPP