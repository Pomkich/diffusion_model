#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <thread>

typedef std::vector<std::vector<sf::RectangleShape>> array_rect;
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

void calculate_therm(array_pt& prev_iter, array_pt& next_iter, array& con_coef, array& therm_pts) {
    // init constants
    const int h = 1;
    const int dt = 1;

    int x_prev, y_prev;
    int x_next, y_next;

    for (int i = 0; i < prev_iter->size(); i++) {
        for (int j = 0; j < (*prev_iter)[i].size(); j++) {
            // avoiding going outside the array
            x_prev = i - 1; y_prev = j - 1;
            x_next = i + 1; y_next = j + 1;
            x_prev = clamp(x_prev, 0, prev_iter->size() - 1);
            y_prev = clamp(y_prev, 0, (*prev_iter)[i].size() - 1);
            x_next = clamp(x_next, 0, prev_iter->size() - 1);
            y_next = clamp(y_next, 0, (*prev_iter)[i].size() - 1);

            // calculation of the next iteration
            (*next_iter)[i][j] = (*prev_iter)[i][j] + con_coef[i][j] * (
                (*prev_iter)[i][y_prev] + (*prev_iter)[i][y_next] +
                (*prev_iter)[x_prev][j] + (*prev_iter)[x_next][j] + 
                (*prev_iter)[i][j]) / 5 / 
                (h * h) * dt + therm_pts[i][j] * dt;

            if ((*next_iter)[i][j] > 255) {
                (*next_iter)[i][j] = 255;
            }
        }
    }
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

    // first 20 cells have conductivity = 2
    for (int i = 0; i < 20; i++) {
        conductivity_coef.push_back(std::vector<int>());
        thermal_points.push_back(std::vector<int>());
        for (int j = 0; j < conductivity_coef[i].size(); j++) {
            conductivity_coef[i][j] = 2;
        }
    }
    
    // setup thermal points
    //thermal_points[10][10] = 1;
    thermal_points[50][30] = 1;

    const int screen_width = 800;
    const int screen_height = 600;

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "thermal simulation");
    // init rectagnles
    array_rect rectangles;
    for (int i = 0; i < first_buffer->size(); i++) {
        rectangles.push_back(std::vector<sf::RectangleShape>());
        for (int j = 0; j < (*first_buffer)[i].size(); j++) {
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(10, 10));
            rect.setFillColor(sf::Color::Black);
            rect.setPosition(j * 10, i * 10);
            rectangles[i].push_back(rect);
        }
    }

    int pause = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for (int i = 0; i < rectangles.size(); i++) {
            for (int j = 0; j < rectangles[i].size(); j++) {
                window.draw(rectangles[i][j]);
            }
        }
        window.display();
        // recalculate temperature
        calculate_therm(first_buffer, second_buffer, conductivity_coef, thermal_points);
        for (int i = 0; i < rectangles.size(); i++) {
            for (int j = 0; j < rectangles[i].size(); j++) {
                sf::Color color;    // setup new color
                color.r = (*second_buffer)[i][j];
                rectangles[i][j].setFillColor(color);
            }
        }
        swap(first_buffer, second_buffer);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}