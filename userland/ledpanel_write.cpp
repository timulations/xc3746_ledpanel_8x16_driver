#include <thread>
#include <chrono>
#include <ctime>

#include "ledpanel_user.h"
#include "ledpanel_bitmaps.h"

/* Example: display the current 24 hour time */
#define APPLY_COORDS(gen, coord_arr) gen.set_coords(coord_arr.data(), coord_arr.size())

/* Render the current time into HH MM format */
static LedPanel::Grid generate_current_time(int hour, int minute) {

    LedPanel::GridGenerator gen;
    APPLY_COORDS(gen, sevensegs[minute % 10]);
    gen.shift_right(4);
    APPLY_COORDS(gen, sevensegs[minute / 10]);
    gen.shift_right(5);
    APPLY_COORDS(gen, sevensegs[hour % 10]);
    gen.shift_right(4);
    APPLY_COORDS(gen, sevensegs[hour / 10]);

    return gen.get_grid();
}

int main(int argc, char *argv[]) {
    
    using namespace LedPanel;

    LedPanelRenderer renderer;

    int last_minute = -1;
    int curr_minute, curr_hour;

    Grid curr_grid;

    /* main rendering loop */
    while (true) {
        auto now = std::chrono::system_clock::now();
        std::time_t curr_time = std::chrono::system_clock::to_time_t(now);
        std::tm* local_time = std::localtime(&curr_time);

        curr_minute = local_time->tm_min;
        curr_hour = local_time->tm_hour;

        if (curr_minute != last_minute) {
            curr_grid = generate_current_time(curr_hour, curr_minute);
        }

        renderer.draw(curr_grid);
    }

    return 0;
}