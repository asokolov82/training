#pragma once
#include <algorithm>
#include <iostream>

using namespace std;

class IntArray
{
private:
    int _size{};
    int* _data{};
   

public:
    IntArray() = default;
    IntArray(int size);
    ~IntArray();
    int& operator[](int pos);
    int getSize() const;
    int search(int value);
    void erase();
    void reAllocate(int newSize);
    void resize(int newSize);
    void remove(int pos);
    void insert(int value, int pos);
    void insertAtBeginning(int value) { insert(value, 0); }
    void insertAtEnd(int value) { insert(value, _size); }
    IntArray& operator=(const IntArray& a);
};

class bad_range : public exception 
{
public:
    virtual const char* what() const noexcept override;
   
};

class bad_lenght : public exception
{
public:
    virtual const char* what() const noexcept override;

};

void print(IntArray& a);
