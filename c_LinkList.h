#pragma once
#include <iostream>
#include <cmath>
#include <cstring>
#include <sstream>
#include "AVLTree.h"
#include "UtilityFunctions.h"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
using namespace std;

template <typename T>
struct CNode;

template <typename T>
class FingerTable
{
private:
    struct FNode
    {
        FNode* next, * prev;
        CNode<T>* reference;
        FNode(CNode<T>* ref)
        {
            next = prev = NULL;
            reference = ref;
        }
    };
    FNode* head, * tail;
    int bits;
public:
    FingerTable(int b) : head(NULL), tail(NULL), bits(b)
    {
    }
    /* Functions Maintaining Doubly Linked List of Machines References */
    CNode<T>* controllerId(string* nodeIds, string id, CNode<T>* h, int256_t len)
    {
        CNode<T>* temp = h;
        for (int64_t i = 0; i < len; i++)
        {
            if (i == len - 1)
            {
                return h;
            }
            if (stoint(id) > stoint(nodeIds[i]) && stoint(id) <= stoint(nodeIds[i + 1]))
            {
                return temp->next;
            }
            temp = temp->next;
        }
        return NULL;
    }
    void append(CNode<T>* h, CNode<T>* curr, string* nodeIds, int256_t machines)
    {
        head = tail = NULL;
        int i = 0;
        do
        {
            int256_t id_val = stoint(curr->node_id) + power(2, i);
            id_val = (id_val > power(2, bits) - 1) ? (id_val - (power(2, bits) - 1)) : id_val;
            ostringstream str1;
            str1 << id_val;
            string id = str1.str();
            CNode<T>* ref = controllerId(nodeIds, id, h, machines);
            FNode* newNode = new FNode(ref);
            if (head == NULL)
            {
                head = newNode;
                tail = head;
            }
            else
            {
                tail->next = newNode;
                newNode->prev = tail;
                tail = tail->next;
            }
            i++;
        } while (i < bits);
    }
    void displayFT()
    {
        FNode* temp = head;
        cout << "Finger Table : ";
        while (temp)
        {
            cout << temp->reference->node_id << " , ";
            temp = temp->next;
        }
        cout << endl;
    }
    /* Get Machine Node_id or reference */
    T get(int n)
    {
        FNode* temp = head;
        int i = 0;
        while (temp)
        {
            if (i == n)
            {
                return temp->reference->node_id;
            }
            i++;
            temp = temp->next;
        }
        return "";
    }
    CNode<T>* getref(int n)
    {
        FNode* temp = head;
        int i = 0;
        while (temp)
        {
            if (i == n)
            {
                return temp->reference;
            }
            temp = temp->next;
            i++;
        }
        return NULL;
    }
};

template <typename T>
struct CNode
{
    CNode* next, * prev;
    T node_id;
    AVLTree<T> tree;
    FingerTable<T>* FT;
    CNode(T id, int bits) : node_id(id), next(NULL), prev(NULL)
    {
        FT = new FingerTable<T>(bits);
    }
};

template <typename T>
class c_LinkList
{
private:
    CNode<T>* head, * tail;
    int64_t len;
    int nbits;

public:
    c_LinkList() : head(NULL), tail(NULL), len(0), nbits(0)
    {
    }
    /* Function to join Machines together in circular linked list */
    void append(T id, int bits)
    {
        nbits = bits;
        CNode<T>* newNode = new CNode<T>(id, bits);
        if (isEmpty())
        {
            head = newNode;
            tail = head;
            head->prev = tail;
            tail->next = head;
            len++;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = tail->next;
            tail->next = head;
            head->prev = tail;
            len++;
        }
    }
    /* Utility Functions */
    void print()
    {
        CNode<T>* tmp = head;
        do
        {
            cout << "NODE : " << tmp->node_id << endl;
            tmp->FT->displayFT();
            if (tmp->tree.root)
            {
                cout << "AVL :";
                tmp->tree.Inorder(tmp->tree.root);
                cout << endl;
            }
            tmp = tmp->next;
        } while (tmp != head && tmp);
    }
    void printFingerTable(CNode<T>* SMachine)
    {
        CNode<T>* tmp = SMachine;
        cout << "NODE : " << tmp->node_id << endl;
        tmp->FT->displayFT();
    }
    void displayAVL(CNode<T>* SMachine)
    {
        CNode<T>* tmp = SMachine;
        cout << "NODE : " << tmp->node_id << endl;
        if (tmp->tree.root)
        {
            cout << "AVL ---> ";
            tmp->tree.Inorder(tmp->tree.root);
            cout << endl;
        }
        else
        {
            cout << "Machine has no data." << endl;
        }
        cout << endl;
    }
    void display()
    {
        CNode<T>* tmp = head;
        int i = 1;
        do
        {
            cout << "Machine ID (" << i << ") : " << tmp->node_id << endl;
            tmp = tmp->next;
            i++;
        } while (tmp != head && tmp);
        cout << "Tail : " << tail->node_id << " , head : " << head->node_id << endl;
    }
    /* Add a new machine on user demand */
    void newMachine(T idofMachine, T* nodeIds)
    {
        CNode<T>* temp = head;
        while (1)
        { 
            if ((stoint(temp->node_id) < stoint(idofMachine) && stoint(temp->next->node_id) > stoint(idofMachine)) || (temp == tail && stoint(temp->node_id) < stoint(idofMachine)) || (temp == head && stoint(temp->node_id) > stoint(idofMachine)))
            {
                if(temp == head && stoint(temp->node_id) > stoint(idofMachine)){
                    CNode<T>*  machine = temp->prev;
                    temp->prev = new CNode<T>(idofMachine, nbits);
                    temp->prev->next = temp;
                    machine->next = temp->prev;
                    temp->prev->prev = machine;
                    head = temp->prev;
                    len++;
                    machine = temp;
                    temp = temp->prev;
                    generateFingerTable(nodeIds, len);
                    int256_t tmpid = stoint(idofMachine);
                    while (tmpid >= 0)
                    {
                        ostringstream str1;
                        str1 << tmpid;
                        LinkedList<T> get = machine->tree.search(machine->tree.root, str1.str());
                        if (get.getLength() > 0)
                        {
                            int i = 0;
                            while (i < get.getLength())
                            {
                                temp->tree.InsertNode(get.get(i), str1.str());
                                i++;
                            }
                            machine->tree.remove(machine->tree.root, str1.str());
                        }
                        tmpid -= 1;
                        if (tmpid < 0) {
                            tmpid = power(2, nbits) - 1;
                            while (tmpid > stoint(tail->node_id))
                            {
                                ostringstream str1;
                                str1 << tmpid;
                                LinkedList<T> get = machine->tree.search(machine->tree.root, str1.str());
                                if (get.getLength() > 0)
                                {
                                    int i = 0;
                                    while (i < get.getLength())
                                    {
                                        temp->tree.InsertNode(get.get(i), str1.str());
                                        i++;
                                    }
                                    machine->tree.remove(machine->tree.root, str1.str());
                                }
                                tmpid -= 1;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    CNode<T>* machine = temp->next;
                    temp->next = new CNode<T>(idofMachine, nbits);
                    machine->prev = temp->next;
                    temp->next->next = machine;
                    temp->next->prev = temp;
                    if (temp == tail)
                    {
                        tail = temp->next;
                    }
                    len++;
                    generateFingerTable(nodeIds, len);
                    int256_t tmpid = stoint(idofMachine);
                    while (tmpid > stoint(temp->node_id))
                    {
                        ostringstream str1;
                        str1 << tmpid;
                        LinkedList<T> get = machine->tree.search(machine->tree.root, str1.str());
                        if (get.getLength() > 0)
                        {
                            int i = 0;
                            while (i < get.getLength())
                            {
                                temp->next->tree.InsertNode(get.get(i), str1.str());
                                i++;
                            }
                            machine->tree.remove(machine->tree.root, str1.str());
                        }
                        tmpid -= 1;
                    }
                }
                break;
            }
            temp = temp->next;
        }
    }
    /* Remove a new machine on user demand */
    void remMachine(CNode<T>* SMachine, T* nodeIds)
    {
        CNode<T>* temp = SMachine->prev, * tmphead = head;
        temp->next = SMachine->next;
        SMachine->next->prev = temp;
        if (SMachine == head)
        {
            head = SMachine->next;
        }
        if (SMachine == tail)
        {
            tail = SMachine->prev;
        }
        len--; //decrementing number of Machines
        generateFingerTable(nodeIds, len);
        temp = SMachine->next;
        if (SMachine == tmphead)
        {
            int256_t tmpid = stoint(SMachine->node_id);
            while (tmpid >= 0)
            {
                ostringstream str1;
                str1 << tmpid;
                LinkedList<T> get = SMachine->tree.search(SMachine->tree.root, str1.str());
                int i = 0;
                while (i < get.getLength())
                {
                    temp->tree.InsertNode(get.get(i), str1.str());
                    //operations(str1.str(), get.get(i), temp, 0);
                    i++;
                }
                tmpid -= 1;
                if (tmpid < 0)
                {
                    tmpid = power(2, nbits) - 1;
                    while (tmpid > stoint(tail->node_id))
                    {
                        ostringstream str1;
                        str1 << tmpid;
                        LinkedList<T> get = SMachine->tree.search(SMachine->tree.root, str1.str());
                        int i = 0;
                        while (i < get.getLength())
                        {
                            temp->tree.InsertNode(get.get(i), str1.str());
                            //operations(str1.str(), get.get(i), temp, 0);
                            i++;
                        }
                        tmpid -= 1;
                    }
                    break;
                }
            }
        }
        else
        {
            int256_t tmpid = stoint(SMachine->node_id);
            while (tmpid > stoint(SMachine->prev->node_id))
            {
                ostringstream str1;
                str1 << tmpid;
                LinkedList<T> get = SMachine->tree.search(SMachine->tree.root, str1.str());
                int i = 0;
                while (i < get.getLength())
                {
                    temp->tree.InsertNode(get.get(i), str1.str());
                    //operations(str1.str(), get.get(i), temp, 0);
                    i++;
                }
                tmpid -= 1;
            }
        }
    }
    CNode<T>* getMachine(int256_t idx)
    {
        int256_t i = 0;
        CNode<T>* temp = head;
        do
        {
            if (i == idx)
            {
                return temp;
            }
            i++;
            temp = temp->next;
        } while (temp != head && temp);
        return NULL;
    }
    /* Functions responsible for enhancing search, remove, insert algorithm via finger table */
    bool rangeoflocal(T id, T mID, T prevMid)
    {
        if (stoint(prevMid) == stoint(tail->node_id))
        {
            if (stoint(mID) >= stoint(id))
            {
                return true;
            }
            else if (stoint(id) >= stoint(prevMid))
            {
                return true;
            }
        }
        else
        {
            if ((stoint(mID) - stoint(id)) >= 0 && (stoint(mID) - stoint(id)) < (stoint(mID) - stoint(prevMid)))
            {
                return true;
            }
        }
        return false;
    }
    bool rangeoffirst(T id, T Mid, T FT1)
    {
        if (stoint(id) > stoint(Mid))
        {
            if (stoint(id) <= stoint(FT1))
            {
                return true;
            }
            else if (stoint(FT1) == stoint(head->node_id))
            {
                return true;
            }
        }
        return false;
    }
    bool rangeofsecond(T id, T cId, T nId)
    {
        if (stoint(id) > stoint(cId))
        {
            if (stoint(id) <= stoint(nId))
            {
                return true;
            }
            else if ((stoint(id) > stoint(tail->node_id)) && (stoint(nId) == stoint(head->node_id)))
            {
                return true;
            }
        }
        return false;
    }
    void operations(T data_NodeID, T value, CNode<T>* SMachine, int option, int times = 0)
    {
        if (times > len)
        {
            return;
        }
        CNode<T>* temp = SMachine;
        cout << temp->node_id;
        if (rangeoflocal(data_NodeID, temp->node_id, temp->prev->node_id))
        {
            if (option == 0)
            {
                temp->tree.InsertNode(value, data_NodeID);
            }
            else if (option == 1)
            {
                temp->tree.search(temp->tree.root, data_NodeID);
            }
            else if (option == 2)
            {
                temp->tree.remove(temp->tree.root, data_NodeID);
            }
            return;
        }
        else if (rangeoffirst(data_NodeID, temp->node_id, temp->FT->get(0)))
        {
            temp = temp->FT->getref(0);
            cout << " --> " << temp->node_id;
            if (option == 0)
            {
                temp->tree.InsertNode(value, data_NodeID);
            }
            else if (option == 1)
            {
                temp->tree.search(temp->tree.root, data_NodeID);
            }
            else if (option == 2)
            {
                temp->tree.remove(temp->tree.root, data_NodeID);
            }
            return;
        }
        else
        {
            int i = 0;
            while (i < nbits - 1)
            {
                if (rangeofsecond(data_NodeID, temp->FT->get(i), temp->FT->get(i + 1)))
                {
                    cout << " --> ";
                    temp = temp->FT->getref(i);
                    operations(data_NodeID, value, temp, option, times + 1);
                    break;
                }
                else if (!(rangeofsecond(data_NodeID, temp->FT->get(i), temp->FT->get(i + 1))) && i == nbits - 2)
                {
                    cout << " --> ";
                    temp = temp->FT->getref(i + 1);
                    operations(data_NodeID, value, temp, option, times + 1);
                    break;
                }
                i++;
            }
        }
    }
    void generateFingerTable(string* nodeIds, int256_t machines)
    {
        CNode<T>* temp = head;
        int256_t i = 0;
        do
        {
            temp->FT->append(head, temp, nodeIds, machines);
            temp = temp->next;
            i++;
        } while (temp != head && temp);
    }
    bool isEmpty()
    {
        return head == NULL;
    }
    ~c_LinkList() {}
};
