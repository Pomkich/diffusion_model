#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <algorithm>

typedef std::vector<std::vector<int>> array;
typedef std::shared_ptr<std::vector<std::vector<int>>> array_pt;

int clamp(int n, int lower, int upper) {
    return std::max(lower, std::min(n, upper));
}

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
    arr.pop_back();
}

int main()
{
    // buffers contain start temperature
    array_pt first_buffer = std::make_shared<array>();
    array_pt second_buffer = std::make_shared<array>();
    array conductivity_coef;
    array thermal_points;

    read_file("massiv.txt", *first_buffer);
    read_file("massiv.txt", *second_buffer);

    // init conductivity and thermal points
    for (int i = 0; i < first_buffer->size(); i++) {
        conductivity_coef.push_back(std::vector<int>());
        thermal_points.push_back(std::vector<int>());
        for (int j = 0; j < (*first_buffer)[i].size(); j++) {
            conductivity_coef[i].push_back(1);
            thermal_points[i].push_back(0);
        }
    }

    thermal_points[0][3] = 1;

    // init constants
    const int h = 1;
    const int dt = 1;

    int pause = 0;
    for (int x = 0; x < 10; x++) {
        for (int i = 0; i < first_buffer->size(); i++) {
            for (int j = 0; j < (*first_buffer)[i].size(); j++) {
                // avoiding going outside the array
                int x_prev, y_prev;
                int x_next, y_next;
                x_prev = i - 1; y_prev = j - 1;
                x_next = i + 1; y_next = j + 1;
                x_prev = clamp(x_prev, 0, first_buffer->size() - 1);
                y_prev = clamp(y_prev, 0, (*first_buffer)[i].size() - 1);
                x_next = clamp(x_next, 0, first_buffer->size() - 1);
                y_next = clamp(y_next, 0, (*first_buffer)[i].size() - 1);

                // calculation of the next iteration
                (*second_buffer)[i][j] = (*first_buffer)[i][j] + conductivity_coef[i][j] * (
                    (*first_buffer)[i][y_prev] + (*first_buffer)[i][y_next] +
                    (*first_buffer)[x_prev][j] + (*first_buffer)[x_next][j]) / 
                    (h * h) * dt + thermal_points[i][j] * dt;
            }
        }

        for (int i = 0; i < second_buffer->size(); i++) {
            for (int j = 0; j < (*second_buffer)[i].size(); j++) {
                std::cout << (*second_buffer)[i][j] << " ";
            }
            std::cout << std::endl;
        }

        // swtich buffers
        std::swap(first_buffer, second_buffer);

        std::cin >> pause;
    }

    return 0;
}