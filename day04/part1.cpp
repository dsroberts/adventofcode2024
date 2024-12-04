#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cinttypes>
#include <algorithm>

int radial_check(std::vector<std::string> *grid, uint32_t i, uint32_t j)
{

    /*
    S..S..S
    .A.A.A.
    ..MMM..
    SAMXMAS
    ..MMM..
    .A.A.A.
    S..S..S

    If there is an X at (i,j), there are 8 possible XMAS that can
    start from there:
    1) (i,j),(i+1,j),(i+2,j),(i+3,j)         ( 1, 0)
    2) (i,j),(i-1,j),(i-2,j),(i-3,j)         (-1, 0)
    3) (i,j),(i,j+1),(i,j+2),(i,j+3)         ( 0, 1)
    4) (i,j),(i,j-1),(i,j-2),(i,j-3)         ( 0,-1)
    5) (i,j),(i+1,j+1),(i+2,j+2),(i+3,j+3)   ( 1, 1)
    6) (i,j),(i+1,j-1),(i+2,j-2),(i+3,j-3)   ( 1,-1)
    7) (i,j),(i-1,j+1),(i-2,j+2),(i-3,j+3)   (-1, 1)
    8) (i,j),(i-1,j-1),(i-2,j-2),(i-3,j-3)   (-1,-1)
    */
    int count = 0;
    for (auto i_mul = -1; i_mul < 2; ++i_mul)
    {
        for (auto j_mul = -1; j_mul < 2; ++j_mul)
        {
            if (i + 3 * i_mul < 0 || i + 3 * i_mul >= grid->size() || j + 3 * j_mul < 0 || j + 3 * j_mul >= grid->at(0).length())
            {
                continue;
            }
            std::vector<uint32_t> i_coord{i, i + i_mul, i + 2 * i_mul, i + 3 * i_mul};
            std::vector<uint32_t> j_coord{j, j + j_mul, j + 2 * j_mul, j + 3 * j_mul};
            std::string tmp{grid->at(i_coord[0])[j_coord[0]],
                            grid->at(i_coord[1])[j_coord[1]],
                            grid->at(i_coord[2])[j_coord[2]],
                            grid->at(i_coord[3])[j_coord[3]]};
            count += (tmp == std::string{"XMAS"});
        }
    }
    return count;
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    std::vector<std::string> all;

    while (getline(f, line))
    {
        all.push_back(line);
    }
    auto count = 0;
    for (auto i = 0; i < all.size(); ++i)
    {
        for (auto j = 0; j < all[0].length(); ++j)
        {
            if (all[i][j] == 'X')
            {
                count += radial_check(&all, i, j);
            }
        }
    }
    std::cout << count << std::endl;
}