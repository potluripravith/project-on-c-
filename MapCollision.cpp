
#include <algorithm>
#include <array>
#include <chrono>
#include <vector>
#include <cmath>

#include "Headers/Global.hpp"
#include "Headers/MapCollision.hpp"

unsigned char map_collision(float x, float y, const std::vector<Cell>& check_cells, const Map& map) {
    int cell_x = floor(x / CELL_DIMENSION);
    int cell_y = floor(y / CELL_DIMENSION);

    unsigned char result = 0;
    std::array<std::pair<int, int>, 4> cells = {{{cell_x, cell_y}, {cell_x + 1, cell_y}, {cell_x, cell_y + 1}, {cell_x + 1, cell_y + 1}}};

    for (int i = 0; i < 4; ++i) {
        int curr_x = cells[i].first;
        int curr_y = cells[i].second;
        if (curr_x >= 0 && curr_x < map.size() && curr_y >= 0 && curr_y < map[0].size() && 
            std::find(check_cells.begin(), check_cells.end(), map[curr_x][curr_y]) != check_cells.end()) {
            result |= (1 << i);
        }
        else if (std::find(check_cells.begin(), check_cells.end(), Cell::Wall) != check_cells.end()) {
            result |= (1 << i);
        }
    }

    return result;
}
