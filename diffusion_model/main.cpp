#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

typedef std::vector<std::vector<int>> array;
typedef std::shared_ptr<std::vector<std::vector<int>>> array_pt;

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

void switch_buffers(array_pt& p1, array_pt& p2) {
    array_pt temp = p1;
    p1 = p2;
    p2 = temp;
}

int main()
{
    array_pt first_buffer = std::make_shared<std::vector<std::vector<int>>>();
    array_pt second_buffer = std::make_shared<std::vector<std::vector<int>>>();
    read_file("massiv.txt", *first_buffer);
    read_file("massiv.txt", *second_buffer);

    (*first_buffer)[0][0] = 12;
    (*second_buffer)[0][0] = 3;

    for (int i = 0; i < first_buffer->size(); i++) {
        for (int j = 0; j < (*first_buffer)[i].size(); j++) {
            std::cout << (*first_buffer)[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;

    for (int i = 0; i < second_buffer->size(); i++) {
        for (int j = 0; j < (*second_buffer)[i].size(); j++) {
            std::cout << (*second_buffer)[i][j] << " ";
        }
        std::cout << std::endl;
    }

    switch_buffers(first_buffer, second_buffer);
    std::cout << "%%%%%%%%%%%%%%%%%%%%%" << std::endl;

    for (int i = 0; i < first_buffer->size(); i++) {
        for (int j = 0; j < (*first_buffer)[i].size(); j++) {
            std::cout << (*first_buffer)[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;

    for (int i = 0; i < second_buffer->size(); i++) {
        for (int j = 0; j < (*second_buffer)[i].size(); j++) {
            std::cout << (*second_buffer)[i][j] << " ";
        }
        std::cout << std::endl;
    }

    //const delta_time = 1;


    return 0;
}