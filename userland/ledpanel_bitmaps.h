#ifndef _LEDPANEL_BITMAPS_H_
#define _LEDPANEL_BITMAPS_H_

#include <utility>
#include <vector>

/* Numbers */
const std::vector<std::pair<int, int>> sevenseg_zero = {
    {1, 0}, {1, 1}, {1, 2},
    {2, 0},         {2, 2},
    {3, 0},         {3, 2},
    {4, 0},         {4, 2},
    {5, 0}, {5, 1}, {5, 2}
};

const std::vector<std::pair<int, int>> sevenseg_one = {
    {1, 2},
    {2, 2},
    {3, 2},
    {4, 2},
    {5, 2}
};

const std::vector<std::pair<int, int>> sevenseg_two = {
    {1, 0}, {1, 1}, {1, 2},
                    {2, 2},
    {3, 0}, {3, 1}, {3, 2},
    {4, 0},        
    {5, 0}, {5, 1}, {5, 2}
};

const std::vector<std::pair<int, int>> sevenseg_three = {
    {1, 0}, {1, 1}, {1, 2},
                    {2, 2},
    {3, 0}, {3, 1}, {3, 2},
                    {4, 2},       
    {5, 0}, {5, 1}, {5, 2}
};

const std::vector<std::pair<int, int>> sevenseg_four = {
    {1, 0},         {1, 2},
    {2, 0},         {2, 2},
    {3, 0}, {3, 1}, {3, 2},
                    {4, 2},       
                    {5, 2}
};

const std::vector<std::pair<int, int>> sevenseg_five = {
    {1, 0}, {1, 1}, {1, 2},
    {2, 0},
    {3, 0}, {3, 1}, {3, 2},
                    {4, 2},      
    {5, 0}, {5, 1}, {5, 2}
};

const std::vector<std::pair<int, int>> sevenseg_six = {
    {1, 0}, {1, 1}, {1, 2},
    {2, 0},
    {3, 0}, {3, 1}, {3, 2},
    {4, 0},         {4, 2},
    {5, 0}, {5, 1}, {5, 2}
};

const std::vector<std::pair<int, int>> sevenseg_seven = {
    {1, 0}, {1, 1}, {1, 2},
                    {2, 2},
                    {3, 2},
                    {4, 2},        
                    {5, 2}
};

const std::vector<std::pair<int, int>> sevenseg_eight = {
    {1, 0}, {1, 1}, {1, 2},
    {2, 0},         {2, 2},
    {3, 0}, {3, 1}, {3, 2},
    {4, 0},         {4, 2},
    {5, 0}, {5, 1}, {5, 2}
};

const std::vector<std::pair<int, int>> sevenseg_nine = {
    {1, 0}, {1, 1}, {1, 2},
    {2, 0},         {2, 2},
    {3, 0}, {3, 1}, {3, 2},
                    {4, 2},
    {5, 0}, {5, 1}, {5, 2}
};


const std::vector<std::vector<std::pair<int, int>>> sevensegs = {
    sevenseg_zero,
    sevenseg_one,
    sevenseg_two,
    sevenseg_three,
    sevenseg_four,
    sevenseg_five,
    sevenseg_six,
    sevenseg_seven,
    sevenseg_eight,
    sevenseg_nine
};

const std::vector<std::pair<int, int>> bigzero = {
    {2, 0}, {1, 1}, {0, 2}, {0, 3}, {1, 4}, {2, 5},
    {3, 0}, {4, 0}, {5, 0}, {3, 5}, {4, 5}, {5, 5},
    {6, 1}, {7, 2}, {7, 3}, {6, 4}
};

const std::vector<std::pair<int, int>> bigone = {
    {0, 2}, {0, 3},
    {1, 0}, {1, 1}, {1, 2}, {1, 3},
    {2, 2}, {2, 3},
    {3, 2}, {3, 3},
    {4, 2}, {4, 3},
    {5, 2}, {5, 3},
    {6, 2}, {6, 3},
    {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}
}; 

#endif /* _LEDPANEL_BITMAPS_H_ */