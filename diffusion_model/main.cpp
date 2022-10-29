#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>


void read_file(std::string file_name, std::vector<std::vector<int>>& arr) {
    std::ifstream file(file_name);

    arr.push_back(std::vector<int>());  // initialization
    std::string row;
    int i = 0;
    while (std::getline(file, row)) {
        std::stringstream ss(row);
        int value = 0;
        while (ss >> value)
            arr[i].push_back(value);
        arr.push_back(std::vector<int>());
        i++;
    }
}

int main()
{
    std::vector<std::vector<int>> sectors;
    read_file("massiv.txt", sectors);

    for (auto& krow : sectors) {
        for (auto& val : krow) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}