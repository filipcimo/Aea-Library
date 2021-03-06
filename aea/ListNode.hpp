#ifndef LISTNODE_HPP
#define LISTNODE_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>


    
namespace aea
{ 
    template<typename T>
    class ListNode
    {
        public:
            ListNode() = default;
            ListNode(const T& item);
            ListNode(T&& item);
            ListNode(const ListNode<T>& obj);
            ListNode(ListNode<T>&& obj);
            ~ListNode();

            ListNode<T>& operator=(const T& item);
            ListNode<T>& operator=(T&& item);
            ListNode<T>& operator=(const ListNode<T>& obj);
            ListNode<T>& operator=(ListNode<T>&& obj);

            void reset();
            T* dataNode() const;
            ListNode<T>* nextNode() const;
            void nextNode(ListNode<T>* ptr);


        protected:
            T* data = nullptr;
            ListNode<T>* next = nullptr;
    };



    template<typename T>
    ListNode<T>::~ListNode()
    {
        if (data != nullptr)
        {
            delete data;
        }
    }


    template<typename T>
    ListNode<T>::ListNode(const T& item)
    {
        data = new T(item);
    }


    template<typename T>
    ListNode<T>::ListNode(T&& item)
    {
        data = new T(std::move(item));
    }


    template<typename T>
    ListNode<T>::ListNode(const ListNode<T>& obj)
    {
        data = new T(*obj.data);
        next = obj.next;
    }


    template<typename T>
    ListNode<T>::ListNode(ListNode<T>&& obj)
    {
        data = obj.data;
        next = obj.next;

        obj.data = nullptr;
        obj.next = nullptr;
    }


    template<typename T>
    ListNode<T>& ListNode<T>::operator=(const T& item)
    {
        if (data == nullptr) { data = new T(); }

        *data = item;
        return *this;
    }


    template<typename T>
    ListNode<T>& ListNode<T>::operator=(T&& item)
    {
        if (data == nullptr) { data = new T(); }

        *data = std::move(item);
        return *this;
    }


    template<typename T>
    ListNode<T>& ListNode<T>::operator=(const ListNode<T>& obj)
    {
        if (data == nullptr)
        {
            data = new T();
        }

        if (next == nullptr)
        {
            next = new ListNode<T>();
        }

        *data = *obj.data;
        *next = *obj.next;

        return *this;
    }


    template<typename T>
    ListNode<T>& ListNode<T>::operator=(ListNode<T>&& obj)
    {
        data = obj.data;
        next = obj.next;

        obj.data = nullptr;
        obj.next = nullptr;

        return *this;
    }


    template<typename T>
    void ListNode<T>::reset()
    {
        if (data != nullptr)
        {
            delete data;
            data = nullptr;
        }

        next = nullptr;
    }


    template<typename T>
    T* ListNode<T>::dataNode() const
    {
        return data;
    }


    template<typename T>
    ListNode<T>* ListNode<T>::nextNode() const
    {
        return next;
    }


    template<typename T>
    void ListNode<T>::nextNode(ListNode<T>* ptr)
    {
        next = ptr;
    }
}



#endif  // LISTNODE_HPP