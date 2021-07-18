#pragma once
#include <iostream>
#include <cmath>
#include <cstring>
#include <sstream>
#include "UtilityFunctions.h"
#include "LinkList.h"
using namespace std;
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;


template <class T>
class Node
{
public:
    LinkedList<T> val; // value
    T nodeId;
    int height;
    Node<T>* left, * right, * parent;
    Node(T value, T id)
    {
        height = 0;
        val.insert(value);
        nodeId = id;
        left = right = parent = NULL;
    }
    Node(LinkedList<T> v, T id)
    {
        val = v;
        nodeId = id;
        left = right = parent = NULL;
        height = 0;
    }
    void insert(T value)
    {
        val.insert(value);
    }
};


template <typename T>
class AVLTree
{
public:
    Node<T>* root;
    AVLTree() : root(NULL) {}
    int max(int a, int b) { return (a > b ? a : b); }
    /* Insert Node in AVL Tree */
    void InsertNode(T val, T id)
    {
        if (root == NULL)
        {
            root = new Node<T>(val, id);
        }
        else
        {
            Node<T>* linker = root;
            Node<T>* newnode = new Node<T>(val, id);
            while (linker != NULL)
            {
                if (stoint(linker->nodeId) > stoint(id))
                {
                    if (linker->left == NULL)
                    {
                        linker->left = newnode;
                        newnode->parent = linker;
                        break;
                    }
                    else
                    {
                        linker = linker->left;
                    }
                }
                else if (stoint(linker->nodeId) == stoint(id))
                {
                    linker->insert(val);
                    break;
                }
                else
                {
                    if (linker->right == NULL)
                    {
                        linker->right = newnode;
                        newnode->parent = linker;
                        break;
                    }
                    else
                    {
                        linker = linker->right;
                    }
                }
            }
            Rebalancer(newnode);
        }
    }
    /* Get height of a specific node */
    int GetHeight(Node<T>* temp) { return ((temp == NULL) ? -1 : temp->height); }
    /* Utility Functions to balance tree*/
    int BalanceFactor(Node<T>* temp) { return (GetHeight(temp->right) - GetHeight(temp->left)); }
    void HeightBalance(Node<T>* temp)
    {
        int l = -1, r = -1;
        if (temp->left)
        {
            l = temp->left->height;
        }
        if (temp->right)
        {
            r = temp->right->height;
        }
        temp->height = max(l, r) + 1;
    }
    void TreeFix(Node<T>* temp)
    {
        if (BalanceFactor(temp) == 2)
        {
            if (BalanceFactor(temp->right) < 0)
            {
                RightRotate(temp->right);
            }
            LeftRotate(temp);
            HeightBalance(temp);
        }
        else if (BalanceFactor(temp) == -2)
        {
            if (BalanceFactor(temp->left) > 0)
            {
                LeftRotate(temp->left);
            }
            RightRotate(temp);
            HeightBalance(temp);
        }
    }
    void Rebalancer(Node<T>* temp)
    {
        if (temp == root)
        {
            TreeFix(root);
        }
        else
        {
            while (temp != NULL)
            {
                HeightBalance(temp);
                temp = temp->parent;
                if (temp)
                {
                    TreeFix(temp);
                }
            }
        }
    }
    void LeftRotate(Node<T>* x)
    {
        Node<T>* nw_node = new Node<T>(x->val, x->nodeId);
        if (x->right->left)
        {
            nw_node->right = x->right->left;
        }
        nw_node->left = x->left;
        x->val = x->right->val;
        x->nodeId = x->right->nodeId;

        x->left = nw_node;
        if (nw_node->left)
        {
            nw_node->left->parent = nw_node;
        }
        if (nw_node->right)
        {
            nw_node->right->parent = nw_node;
        }
        nw_node->parent = x;

        if (x->right->right)
        {
            x->right = x->right->right;
        }
        else
        {
            x->right = NULL;
        }

        if (x->right)
        {
            x->right->parent = x;
        }

        HeightBalance(x->left);
        if (x->right)
        {
            HeightBalance(x->right);
        }
        HeightBalance(x);
    }
    void RightRotate(Node<T>* x)
    {
        Node<T>* nw_node = new Node<T>(x->val, x->nodeId);
        if (x->left->right)
        {
            nw_node->left = x->left->right;
        }
        nw_node->right = x->right;
        x->val = x->left->val;
        x->nodeId = x->left->nodeId;

        x->right = nw_node;
        if (nw_node->left)
        {
            nw_node->left->parent = nw_node;
        }
        if (nw_node->right)
        {
            nw_node->right->parent = nw_node;
        }
        nw_node->parent = x;

        if (x->left->left)
        {
            x->left = x->left->left;
        }
        else
        {
            x->left = nullptr;
        }

        if (x->left)
        {
            x->left->parent = x;
        }

        HeightBalance(x->right);
        if (x->left)
        {
            HeightBalance(x->left);
        }
        HeightBalance(x);
    }
    /* return Linked List of data at specific key */
    LinkedList<T> search(Node<T>* root, T id)
    {
        if (!root)
        {
            return LinkedList<T>();
        }
        if (stoint(id) == stoint(root->nodeId))
        {
            cout << endl;
            return root->val;
        }
        if (stoint(id) > stoint(root->nodeId))
        {
            return search(root->right, id);
        }
        else
        {
            return search(root->left, id);
        }
        return LinkedList<T>();
    }
    /* Display AVL Tree (Inorder) */
    void Inorder(Node<T>* r)
    {
        if (r)
        {
            Inorder(r->left);
            r->val.display();
            Inorder(r->right);
        }
    }
    /* Removal of a node from Tree and rebalancing tree */
    void RemoveNode(Node<T>* Parent, Node<T>* curr, T id)
    {
        if (curr == NULL)
        {
            return;
        }
        if (stoint(id) == stoint(curr->nodeId))
        {
            //Leaf Node
            if (curr->left == NULL && curr->right == NULL)
            {
                if (stoint(Parent->nodeId) == stoint(curr->nodeId))
                {
                    root = NULL;
                }
                else if (Parent->right == curr)
                {
                    Parent->right = NULL;
                }
                else
                {
                    Parent->left = NULL;
                }
                Rebalancer(Parent);
            }
            //LEFT IS AVALIABLE
            else if (curr->left != NULL && curr->right == NULL)
            {
                T tmpid = curr->nodeId;
                LinkedList<T> tmp = curr->val;
                curr->nodeId = curr->left->nodeId;
                curr->val = curr->left->val;
                curr->left->nodeId = tmpid;
                curr->left->val = tmp;
                RemoveNode(curr, curr->left, id);
            }
            else if (curr->left == NULL && curr->right != NULL)
            {
                T tmpid = curr->nodeId;
                LinkedList<T> tmp = curr->val;
                curr->nodeId = curr->right->nodeId;
                curr->val = curr->right->val;
                curr->right->nodeId = tmpid;
                curr->right->val = tmp;
                RemoveNode(curr, curr->right, id);
            }
            //CASE -- 3
            else
            {
                Node<T>* temp = curr->right;
                bool flag = false;
                while (temp->left)
                {
                    flag = true;
                    Parent = temp;
                    temp = temp->left;
                }
                if (!flag)
                {
                    Parent = curr;
                }
                T tmpid = curr->nodeId;
                LinkedList<T> tmp = curr->val;
                curr->nodeId = temp->nodeId;
                curr->val = temp->val;
                temp->nodeId = tmpid;
                temp->val = tmp;
                RemoveNode(Parent, temp, temp->nodeId);
            }
        }
    }
    void remove(Node<T>* root, T id)
    {
        Node<T>* temp = root;
        Node<T>* Parent = temp;
        bool flag = false;
        if (temp == NULL)
        {
            RemoveNode(NULL, NULL, id);
        }
        while (temp)
        {
            if (stoint(id) == stoint(temp->nodeId))
            {
                flag = true;
                RemoveNode(Parent, temp, id);
                break;
            }
            else if (stoint(id) < stoint(temp->nodeId))
            {
                Parent = temp;
                temp = temp->left;
            }
            else
            {
                Parent = temp;
                temp = temp->right;
            }
        }
    }
};
