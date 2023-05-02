#include "vector.h"
#include <iostream>

int main()
{
    my_stl::vector<std::string> vector;
    for (char c = 'a'; c <= 'z'; c++)
    {
        vector.push_back(std::string(1, c));
        std::cout << vector.size() << ' ' << vector.capacity() << '\n';
        for (const auto& item: vector)
        {
            std::cout << item << ' ';
        }
        std::cout << '\n';
    }
    my_stl::vector<std::string> vector1;
    std::swap(vector, vector1);
    for (const auto& item: vector1)
    {
        std::cout << item << ' ';
    }
    std::cout << '\n';

}
