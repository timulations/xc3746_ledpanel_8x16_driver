#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <stdexcept>

#include "ledpanel_user.h"

namespace LedPanel {

LedPanelRenderer::LedPanelRenderer(const std::string& device_path)
    :_device_path{device_path}
    ,_fd{open(_device_path.c_str(), O_WRONLY)} {

    if (_fd < 0) {
        throw std::runtime_error("Could not open path " + _device_path);
    }
}

LedPanelRenderer::~LedPanelRenderer() {
    close(_fd);
}

void LedPanelRenderer::draw(const Grid& grid) {
    /* convert to bitmap */
    unsigned char bitmap[LEDPANEL_WIDTH] = {0};

    for (int i = 0; i < LEDPANEL_WIDTH; ++i) {
        unsigned char byteval = 0;

        for (int j = 0; j < LEDPANEL_HEIGHT; ++j) {
            if (grid[i][j]) {
                byteval |= (1 << j);
            }
        }

        bitmap[i] = byteval;
    }

    write(_fd, bitmap, LEDPANEL_WIDTH);
}


GridGenerator::GridGenerator(): _grid{} {}

void GridGenerator::set_coord(int x, int y) {
    _grid[y][x] |= true;
}

void GridGenerator::unset_coord(int x, int y) {
    _grid[y][x] &= false;
}

void GridGenerator::rotate_left() {
    auto temp = _grid[0];

    for (int i = 1; i < LEDPANEL_WIDTH; ++i) {
        _grid[i - 1] = _grid[i];
    }

    _grid.back() = temp;
}

void GridGenerator::rotate_left(size_t shift) {
    for (int i = 0; i < shift; ++i) {
        rotate_left();
    }
}

void GridGenerator::shift_left() {
    for (int i = 1; i < LEDPANEL_WIDTH; ++i) {
        _grid[i - 1] = _grid[i];
    }

    std::fill(_grid.back().begin(), _grid.back().end(), false);
}

void GridGenerator::shift_left(size_t shift) {
    for (int i = 0; i < shift; ++i) {
        shift_left();
    }
}

void GridGenerator::rotate_right() {
    auto temp = _grid.back();

    for (int i = LEDPANEL_WIDTH - 2; i >= 0; --i) {
        _grid[i + 1] = _grid[i];
    }

    _grid.front() = temp;
}

void GridGenerator::rotate_right(size_t shift) {
    for (int i = 0; i < shift; ++i) {
        rotate_right();
    }
}

void GridGenerator::shift_right() {
    for (int i = LEDPANEL_WIDTH - 2; i >= 0; --i) {
        _grid[i + 1] = _grid[i];
    }

    std::fill(_grid.front().begin(), _grid.front().end(), false);
}

void GridGenerator::shift_right(size_t shift) {
    for (int i = 0; i < shift; ++i) {
        shift_right();
    }
}

void GridGenerator::set_coords(const std::pair<int, int>* coords, size_t num_coords) {
    for (size_t i = 0; i < num_coords; ++i) {
        set_coord(coords[i].first, coords[i].second);
    }
}

void GridGenerator::set_all() {
    for (int i = 0; i < LEDPANEL_WIDTH; ++i) {
        std::fill(_grid[i].begin(), _grid[i].end(), true);
    }
}

void GridGenerator::unset_all() {
    for (int i = 0; i < LEDPANEL_WIDTH; ++i) {
        std::fill(_grid[i].begin(), _grid[i].end(), false);
    }
}

const Grid& GridGenerator::get_grid() const {
    return _grid;
}

Grid& GridGenerator::get_grid() {
    return _grid;
}

GridGenerator GridGenerator::operator|=(const GridGenerator& other) {
    GridGenerator ret(*this);
    for (int i = 0; i < LEDPANEL_WIDTH; ++i) {
        for (int j = 0; j < LEDPANEL_HEIGHT; ++j) {
            ret._grid[i][j] |= other._grid[i][j];
        }
    }
    return ret;
}

GridGenerator GridGenerator::operator&=(const GridGenerator& other) {
    GridGenerator ret(*this);
    for (int i = 0; i < LEDPANEL_WIDTH; ++i) {
        for (int j = 0; j < LEDPANEL_HEIGHT; ++j) {
            ret._grid[i][j] &= other._grid[i][j];
        }
    }
    return ret;
}
}
