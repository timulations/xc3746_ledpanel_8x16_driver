#ifndef _LEDPANEL_USER_H_
#define _LEDPANEL_USER_H_

#include <array>
#include <string>

#define LEDPANEL_DEFAULT_DEVICE_PATH    "/dev/ledpanel"
#define LEDPANEL_HEIGHT                 8
#define LEDPANEL_WIDTH                  16

namespace LedPanel {

/* inside = columns, outside = rows */
using Grid = std::array<std::array<bool, LEDPANEL_HEIGHT>, LEDPANEL_WIDTH>;

struct GridGenerator {
    GridGenerator();

    GridGenerator operator|=(const GridGenerator& other);
    GridGenerator operator&=(const GridGenerator& other);

    void set_coord(int x, int y);
    void unset_coord(int x, int y);

    /* shift whole grid to the left, and wrap around */
    void rotate_left();
    void rotate_left(size_t shift);

    /* shift whole grid to the left and truncate */
    void shift_left();
    void shift_left(size_t shift);

    void rotate_right();
    void rotate_right(size_t shift);

    void shift_right();
    void shift_right(size_t shift);

    void set_all();
    void unset_all();

    void set_coords(const std::pair<int, int>* coords, size_t num_coords);

    const Grid& get_grid() const;
    Grid& get_grid();

private:
    Grid _grid;
};

class LedPanelRenderer {
public:
    LedPanelRenderer(const std::string& device_path = LEDPANEL_DEFAULT_DEVICE_PATH);
    ~LedPanelRenderer();

    LedPanelRenderer(const LedPanelRenderer&) = default;
    LedPanelRenderer& operator=(const LedPanelRenderer&) = default;

    LedPanelRenderer(LedPanelRenderer&&) = default;
    LedPanelRenderer& operator=(LedPanelRenderer&&) = default;

    /* draw the given bitmap */
    void draw(const Grid& grid);

private:
    std::string _device_path;
    int _fd;
};
}

#endif /* _LEDPANEL_USER_H_ */