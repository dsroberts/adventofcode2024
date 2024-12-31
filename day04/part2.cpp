#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cinttypes>
#include <algorithm>

int radial_check(std::vector<std::string> &grid, uint32_t i, uint32_t j)
{

    /*
    M.M M.S S.M S.S
    .A. .A. .A. .A.
    S.S M.S S.M M.M

    If there is an A at (i,j), there are 4 possible X-MAS that can
    start from there:
    */
    int count = 0;
    for (auto i_mul = -1; i_mul < 2; i_mul += 2)
    {
        for (auto j_mul = -1; j_mul < 2; j_mul += 2)
        {
            std::vector<uint32_t> i_coord{i - i_mul, i, i + i_mul};
            std::vector<uint32_t> j_coord{j - j_mul, j, j + j_mul};
            std::string tmp{grid.at(i_coord[0])[j_coord[0]],
                            grid.at(i_coord[1])[j_coord[1]],
                            grid.at(i_coord[2])[j_coord[2]]};
            count += (tmp == std::string{"MAS"});
        }
    }
    return count == 2;
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
    for (auto i = 1; i < all.size() - 1; ++i)
    {
        for (auto j = 1; j < all[0].length() - 1; ++j)
        {
            if (all[i][j] == 'A')
            {
                count += radial_check(all, i, j);
            }
        }
    }
    std::cout << count << std::endl;
}