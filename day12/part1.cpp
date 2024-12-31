#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

class Coord
{
public:
    uint32_t x;
    uint32_t y;
    Coord(uint32_t i, uint32_t j) : x{i},
                                    y{j}
    {
    }
    Coord &operator+=(const Coord &other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    Coord operator+(const Coord &other)
    {
        auto tmp{*this};
        return tmp += other;
    }
    bool operator==(const Coord other)
    {
        return x == other.x && y == other.y;
    }

private:
};

struct grid_bounds
{
    uint32_t nx;
    uint32_t ny;
    bool operator()(uint32_t x, uint32_t y) { return x < nx &&
                                                     x >= 0 &&
                                                     y < ny &&
                                                     y >= 0; }
    bool operator()(Coord c) { return c.x < nx &&
                                      c.x >= 0 &&
                                      c.y < ny &&
                                      c.y >= 0; }
};

Coord contribution(Coord pt, const std::vector<std::string> &all, grid_bounds &in_grid, std::vector<Coord> &found)
{
    // coord.x = area
    // coord.y = perimeter
    auto out = Coord(1, 0);
    found.push_back(pt);

    std::vector<Coord> neighbours = {pt + Coord(1, 0),
                                     pt + Coord(0, 1),
                                     pt + Coord(-1, 0),
                                     pt + Coord(0, -1)};
    for (auto neighbour : neighbours)
    {
        {
            if (!in_grid(neighbour))
            {
                out.y++;
            }
            else if (all[neighbour.y][neighbour.x] != all[pt.y][pt.x])
            {
                out.y++;
            }
            else
            {
                if (std::find(found.begin(), found.end(), neighbour) == found.end())
                {
                    out += contribution(neighbour, all, in_grid, found);
                }
            }
        }
    }
    return out;
}

int main()
{

    std::ifstream f("input.txt");
    std::string line;
    std::vector<std::string> all;
    auto col_ctr = 0u;
    std::vector<uint32_t> prices;
    std::vector<Coord> found;

    while (getline(f, line))
    {
        all.push_back(line);
        col_ctr++;
    }
    f.close();
    auto price = 0ul;
    grid_bounds in_grid{.nx = static_cast<uint32_t>(all[0].size()), .ny = col_ctr};
    for (auto j = 0u; j < all.size(); ++j)
    {
        for (auto i = 0u; i < all[j].size(); ++i)
        {
            if (std::find(found.begin(), found.end(), Coord{i, j}) == found.end())
            {
                auto tmp = contribution(Coord{i, j}, all, in_grid, found);
                price += (tmp.x * tmp.y);
            }
        }
    }
    std::cout << price << std::endl;
}