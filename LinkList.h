#pragma once
#include <iostream>
#include <cmath>
#include <cstring>
#include <sstream>
using namespace std;
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;


template <typename M>
struct LinkedList
{
    template <typename T>
    struct LNode
    {
        T value;
        LNode<T>* next;
        LNode(T val)
        {
            value = val;
            next = NULL;
        }
    };
    int length = 0;
    LNode<M>* head = NULL;
    /* Insertion in Linked List */
    void insert(M val, int i = 0)
    {
        LNode<M>* newNode = new LNode<M>(val);
        length++;
        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            LNode<M>* cur = head;
            while (cur->next)
            {
                cur = cur->next;
            }
            cur->next = newNode;
        }
    }
    /* Get value from particular node */
    M& get(int idx)
    {
        if (idx <= length)
        {
            LNode<M>* cur = head;
            int i = 0;
            while (cur)
            {
                if (i == idx)
                {
                    return cur->value;
                }
                i++;
                cur = cur->next;
            }
        }
        M* str = new M;
        return *str; // If not found return NULL
    }
    /* Display complete linked list */
    void display()
    {
        LNode<M>* temp = head;
        int i = 0;
        while (temp != NULL)
        {
            if (i == length)
            {
                cout << temp->value << " ";
            }
            else
            {
                cout << temp->value << ", ";
            }
            i++;
            temp = temp->next;
        }
    }
    int getLength()
    {
        return length;
    }
};
