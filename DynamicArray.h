#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

struct StudentResult
{
    std::string studentID;
    std::string name;
    double score;

    void print() const
    {
        std::cout << "[" << studentID << "] " << name << " - Score: " << score << std::endl;
    }
};

template <typename T>
class DynamicArray
{
private:
    T *data;
    int capacity;
    int currentSize;

    void resize()
    {
        capacity = capacity * 2;
        T *newArray = new T[capacity];
        for (int i = 0; i < currentSize; i++)
        {
            newArray[i] = data[i];
        }
        delete[] data;
        data = newArray;
    }

public:
    DynamicArray()
    {
        capacity = 2;
        currentSize = 0;
        data = new T[capacity];
    }

    DynamicArray(const DynamicArray &other)
        : data(new T[other.capacity]), capacity(other.capacity), currentSize(other.currentSize)
    {
        for (int i = 0; i < currentSize; i++)
        {
            data[i] = other.data[i];
        }
    }

    DynamicArray &operator=(const DynamicArray &other)
    {
        if (this != &other)
        {
            DynamicArray copy(other);
            std::swap(data, copy.data);
            std::swap(capacity, copy.capacity);
            std::swap(currentSize, copy.currentSize);
        }
        return *this;
    }

    ~DynamicArray()
    {
        delete[] data;
    }

    void add(T item)
    {
        if (currentSize == capacity)
        {
            std::cout << "Array is full Doubling capacity to " << (capacity * 2) << std::endl;
            resize();
        }
        data[currentSize] = item;
        currentSize++;
    }

    void removeLast()
    {
        if (currentSize == 0)
        {
            std::cout << "Error: Array is already empty. Cannot remove." << std::endl;
            return;
        }

        currentSize--;
    }

    int getSize() const
    {
        return currentSize;
    }

    T get(int index) const
    {
        if (index >= 0 && index < currentSize)
        {
            return data[index];
        }
        throw std::out_of_range("Index out of bounds");
    }

    T &operator[](int index)
    {
        if (index >= 0 && index < currentSize)
        {
            return data[index];
        }
        throw std::out_of_range("Index out of bounds");
    }

    const T &operator[](int index) const
    {
        if (index >= 0 && index < currentSize)
        {
            return data[index];
        }
        throw std::out_of_range("Index out of bounds");
    }
};