#ifndef DSA_LIBRARY_H
#define DSA_LIBRARY_H

#include "helpers.h"

// =====================================================
// SHARED DSA LIBRARY
// =====================================================

// ---------- 1. LINKED LIST ----------
// Each node can hold a title, a text, an extra detail and an image path
struct Node
{
    string title;
    string text;
    string extra;
    string image;
    Node* next;
};

class LinkedList
{
private:
    Node* head;
    int count;

public:
    LinkedList()
    {
        head = NULL;
        count = 0;
    }

    ~LinkedList()
    {
        while (head != NULL)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    int size()
    {
        return count;
    }

    // Position starts from 1. Returns NULL if position is wrong
    Node* getNode(int position)
    {
        if (position < 1 || position > count)
            return NULL;

        Node* current = head;
        for (int i = 1; i < position; i++)
            current = current->next;
        return current;
    }

    // Inserts a node at the given position. Returns the position really used
    int insertAt(int position, string title, string text, string extra, string image)
    {
        if (position < 1 || position > count + 1)
            position = count + 1;

        Node* newNode = new Node;
        newNode->title = title;
        newNode->text = text;
        newNode->extra = extra;
        newNode->image = image;

        if (position == 1)
        {
            newNode->next = head;
            head = newNode;
        }
        else
        {
            Node* before = getNode(position - 1);
            newNode->next = before->next;
            before->next = newNode;
        }
        count++;
        return position;
    }
    

    // Adds a node at the end
    int add(string title, string text = "", string extra = "", string image = "")
    {
        return insertAt(count + 1, title, text, extra, image);
    }

    bool removeAt(int position)
    {
        if (position < 1 || position > count)
            return false;

        Node* toDelete;
        if (position == 1)
        {
            toDelete = head;
            head = head->next;
        }
        else
        {
            Node* before = getNode(position - 1);
            toDelete = before->next;
            before->next = toDelete->next;
        }
        delete toDelete;
        count--;
        return true;
    }

    // Swaps a node with the one before it
    void moveUp(int position)
    {
        if (position < 2 || position > count)
            return;

        Node* first = getNode(position - 1);
        Node* second = first->next;
        swap(first->title, second->title);
        swap(first->text, second->text);
        swap(first->extra, second->extra);
        swap(first->image, second->image);
    }

    // Linear search: position of first node whose title or extra contains the keyword
    int search(string keyword)
    {
        for (int i = 1; i <= count; i++)
        {
            Node* node = getNode(i);
            if (node->title.find(keyword) != string::npos || node->extra.find(keyword) != string::npos)
                return i;
        }
        return 0;
    }

    void show()
    {
        if (count == 0)
        {
            cout << "   (nothing added yet)\n";
            return;
        }
        for (int i = 1; i <= count; i++)
        {
            Node* node = getNode(i);
            cout << "   " << i << ". " << node->title;
            if (node->text != "") cout << " - " << node->text;
            if (node->extra != "") cout << " (" << node->extra << ")";
            cout << "\n";
        }
    }
};

// ---------- 2. ARRAY ----------
// Two parallel arrays: names[] and values[] (marks, rating or level)
class ItemArray
{
private:
    string names[MAX];
    int values[MAX];
    int count;

    void swapItems(int a, int b)
    {
        swap(names[a], names[b]);
        swap(values[a], values[b]);
    }

public:
    ItemArray()
    {
        count = 0;
    }

    int size()
    {
        return count;
    }

    string getName(int index)
    {
        return names[index];
    }

    int getValue(int index)
    {
        return values[index];
    }

    void setName(int index, string name)
    {
        names[index] = name;
    }

    void setValue(int index, int value)
    {
        values[index] = value;
    }

    bool add(string name, int value = 0)
    {
        if (count == MAX)
            return false;
        names[count] = name;
        values[count] = value;
        count++;
        return true;
    }

    void removeAt(int index)
    {
        for (int i = index; i < count - 1; i++)
        {
            names[i] = names[i + 1];
            values[i] = values[i + 1];
        }
        names[count] = "";
        values[count] = 0;
        count--;
    }

    // Linear search: returns index or -1
    int search(string name)
    {
        for (int i = 0; i < count; i++)
        {
            if (names[i] == name)
                return i;
        }
        return -1;
    }

    // Bubble sort: highest value first
    void sortByValue()
    {
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
            {
                if (values[j] < values[j + 1])
                    swapItems(j, j + 1);
            }
        }
    }

    // Bubble sort: A to Z
    void sortByName()
    {
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
            {
                if (names[j] > names[j + 1])
                    swapItems(j, j + 1);
            }
        }
    }

    void show(bool withValues)
    {
        if (count == 0)
        {
            cout << "   (nothing added yet)\n";
            return;
        }
        for (int i = 0; i < count; i++)
        {
            cout << "   " << i + 1 << ". " << names[i];
            if (withValues) cout << " (" << values[i] << ")";
            cout << "\n";
        }
    }
};

// ---------- 3. STACK ----------
// Every action remembers what was changed, so it can be undone
struct Action
{
    string description;
    int field;          // which kind of change it was (each module has its own codes)
    int position;
    string oldTitle;
    string oldText;
    string oldExtra;
    string oldImage;
};

class ActionStack
{
private:
    Action items[MAX];
    int top;

public:
    ActionStack()
    {
        top = -1;
    }

    void push(string description, int field = 0, int position = 0, string oldTitle = "",
              string oldText = "", string oldExtra = "", string oldImage = "")
    {
        if (top == MAX - 1)     // stack full: forget the oldest action
        {
            for (int i = 0; i < top; i++)
                items[i] = items[i + 1];
            top--;
        }
        top++;
        items[top].description = description;
        items[top].field = field;
        items[top].position = position;
        items[top].oldTitle = oldTitle;
        items[top].oldText = oldText;
        items[top].oldExtra = oldExtra;
        items[top].oldImage = oldImage;
    }

    bool pop(Action& action)
    {
        if (top == -1)
            return false;
        action = items[top];
        top--;
        return true;
    }

    // Shows the last 5 actions (newest first) without removing them
    void show()
    {
        if (top == -1)
        {
            cout << "   No actions yet.\n";
            return;
        }
        for (int i = top; i >= 0 && i > top - 5; i--)
            cout << "   - " << items[i].description << "\n";
    }
};

// ---------- 4. QUEUE ----------
// Circular array queue (first in, first out)
class StringQueue
{
private:
    string items[MAX];
    int front;
    int rear;
    int count;

public:
    StringQueue()
    {
        front = 0;
        rear = 0;
        count = 0;
    }

    bool isEmpty()
    {
        return count == 0;
    }

    bool enqueue(string item)
    {
        if (count == MAX)
            return false;
        items[rear] = item;
        rear = (rear + 1) % MAX;
        count++;
        return true;
    }

    string dequeue()
    {
        string item = items[front];
        front = (front + 1) % MAX;
        count--;
        return item;
    }
};

#endif