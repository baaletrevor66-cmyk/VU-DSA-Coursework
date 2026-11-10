#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "DynamicArray.h"

bool loadFromFile(DynamicArray<StudentResult> &students, const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open " << filename << " for reading." << std::endl;
        return false;
    }

    DynamicArray<StudentResult> loadedStudents;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        lineNumber++;
        std::istringstream record(line);
        std::string studentID;
        std::string name;
        std::string extra;
        double score;

        if (!(record >> studentID))
        {
            continue;
        }

        if (!(record >> name >> score) || (record >> extra))
        {
            std::cerr << "Error: Invalid student record on line " << lineNumber
                      << " of " << filename << "." << std::endl;
            return false;
        }

        loadedStudents.add({studentID, name, score});
    }

    if (file.bad())
    {
        std::cerr << "Error: Failed while reading " << filename << "." << std::endl;
        return false;
    }

    if (loadedStudents.getSize() < 10)
    {
        std::cerr << "Error: " << filename << " must contain at least 10 student records."
                  << std::endl;
        return false;
    }

    students = loadedStudents;
    std::cout << "Successfully loaded " << students.getSize()
              << " records from " << filename << "." << std::endl;
    return true;
}

bool saveToFile(const DynamicArray<StudentResult> &students, const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open " << filename << " for writing." << std::endl;
        return false;
    }

    for (int i = 0; i < students.getSize(); i++)
    {
        const StudentResult student = students.get(i);
        file << student.studentID << ' ' << student.name << ' ' << student.score << '\n';
    }

    file.close();
    if (!file)
    {
        std::cerr << "Error: Failed while writing " << filename << "." << std::endl;
        return false;
    }

    std::cout << "Successfully saved " << students.getSize()
              << " records to " << filename << "." << std::endl;
    return true;
}

bool containsStudent(const DynamicArray<StudentResult> &students, const std::string &studentID)
{
    for (int i = 0; i < students.getSize(); i++)
    {
        if (students.get(i).studentID == studentID)
        {
            return true;
        }
    }

    return false;
}

bool compareByID(const StudentResult &a, const StudentResult &b)
{
    return a.studentID < b.studentID;
}

bool compareByScore(const StudentResult &a, const StudentResult &b)
{
    return a.score > b.score;
}

template <typename T, typename Comparator>
void insertionSort(DynamicArray<T> &arr, Comparator comp)
{
    for (int i = 1; i < arr.getSize(); i++)
    {
        T key = arr[i];
        int j = i - 1;

        while (j >= 0 && comp(key, arr[j]))
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

template <typename T>
void swapElements(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T, typename Comparator>
int partition(DynamicArray<T> &arr, int low, int high, Comparator comp)
{
    const int pivotIndex = low + std::rand() % (high - low + 1);
    swapElements(arr[pivotIndex], arr[high]);

    const T pivotValue = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (comp(arr[j], pivotValue))
        {
            i++;
            swapElements(arr[i], arr[j]);
        }
    }

    swapElements(arr[i + 1], arr[high]);
    return i + 1;
}

template <typename T, typename Comparator>
void quickSort(DynamicArray<T> &arr, int low, int high, Comparator comp)
{
    if (low < high)
    {
        const int pivotIndex = partition(arr, low, high, comp);
        quickSort(arr, low, pivotIndex - 1, comp);
        quickSort(arr, pivotIndex + 1, high, comp);
    }
}

int binarySearchByID(const DynamicArray<StudentResult> &arr, const std::string &targetID)
{
    int low = 0;
    int high = arr.getSize() - 1;

    while (low <= high)
    {
        const int mid = low + (high - low) / 2;

        if (arr[mid].studentID == targetID)
        {
            return mid;
        }

        if (arr[mid].studentID < targetID)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1;
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const std::string filename = "students.txt";
    DynamicArray<StudentResult> registerArray;

    if (!loadFromFile(registerArray, filename))
    {
        return 1;
    }

    const StudentResult newStudent = {"VU011", "New_Student", 99.9};
    if (!containsStudent(registerArray, newStudent.studentID))
    {
        std::cout << "Adding new student " << newStudent.studentID << "." << std::endl;
        registerArray.add(newStudent);
    }

    std::cout << "Current Student Register" << std::endl;
    for (int i = 0; i < registerArray.getSize(); i++)
    {
        registerArray.get(i).print();
    }

    std::cout << "TASK A2: Sorting and Searching" << std::endl;
    quickSort(registerArray, 0, registerArray.getSize() - 1, compareByID);
    std::cout << "Array sorted alphabetically by ID." << std::endl;

    const std::string searchTarget = "VU003";
    std::cout << "Searching for Student ID: " << searchTarget << "..." << std::endl;

    const int foundIndex = binarySearchByID(registerArray, searchTarget);
    if (foundIndex != -1)
    {
        std::cout << "Student found at index " << foundIndex << ":" << std::endl;
        registerArray[foundIndex].print();
    }
    else
    {
        std::cout << "Student not found." << std::endl;
    }

    if (!saveToFile(registerArray, filename))
    {
        return 1;
    }

    return 0;
}
