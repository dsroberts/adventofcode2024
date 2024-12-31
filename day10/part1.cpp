#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <ranges>
#include <algorithm>

typedef std::pair<uint32_t, u_int32_t> coord;
struct grid_bounds
{
    int nx;
    int ny;
    bool operator()(int x, int y) { return x < nx &&
                                           x >= 0 &&
                                           y < ny &&
                                           y >= 0; }
};

void find(const std::vector<std::vector<int>> &grid, coord start, coord pt, grid_bounds &in_grid, uint32_t &ctr, std::vector<std::pair<coord, coord>> &startends)
{

    auto val = grid[pt.second][pt.first];
    if (val == 9)
    {
        if (std::ranges::find(startends, std::pair{start, pt}) == startends.end())
        {
            ctr++;
            startends.push_back({start, pt});
        }

        return;
    }
    if (in_grid(pt.second + 1, pt.first))
    {
        if (grid[pt.second + 1][pt.first] == val + 1)
        {
            find(grid, start, coord{pt.first, pt.second + 1}, in_grid, ctr, startends);
        }
    }
    if (in_grid(pt.second - 1, pt.first))
    {
        if (grid[pt.second - 1][pt.first] == val + 1)
        {
            find(grid, start, coord{pt.first, pt.second - 1}, in_grid, ctr, startends);
        }
    }
    if (in_grid(pt.second, pt.first + 1))
    {
        if (grid[pt.second][pt.first + 1] == val + 1)
        {
            find(grid, start, coord{pt.first + 1, pt.second}, in_grid, ctr, startends);
        }
    }
    if (in_grid(pt.second, pt.first - 1))
    {
        if (grid[pt.second][pt.first - 1] == val + 1)
        {
            find(grid, start, coord{pt.first - 1, pt.second}, in_grid, ctr, startends);
        }
    }
}

int main()
{

    std::ifstream f("input.txt");
    std::string line;
    std::vector<std::vector<int>> all;
    std::vector<coord> trail_starts;
    std::vector<std::pair<coord, coord>> ends;
    auto row_ctr = 0;
    auto col_ctr = 0;

    while (getline(f, line))
    {
        all.emplace_back();
        row_ctr = 0;
        for (auto i : line)
        {
            all.back().push_back(std::stoul(std::string{i}));
            if (all.back().back() == 0ul)
            {
                trail_starts.emplace_back(row_ctr, col_ctr);
            }
            row_ctr++;
        }
        col_ctr++;
    }
    f.close();
    grid_bounds in_grid{.nx = row_ctr, .ny = col_ctr};

    uint32_t ctr = 0;
    for (auto start : trail_starts)
    {
        find(all, start, start, in_grid, ctr, ends);
    }
    std::cout << ctr << std::endl;
}